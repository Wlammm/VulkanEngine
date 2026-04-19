#include "EnginePch.h"
#include "BLAS.h"

#include "Engine/Vulkan/VulkanAllocator.h"
#include "Engine/Vulkan/VulkanBuffer.h"
#include "Engine/Vulkan/VulkanCommandBuffer.h"
#include "Engine/Vulkan/VulkanContext.h"
#include "Engine/Vulkan/VulkanDevice.h"
#include "Engine/Vulkan/VulkanPhysicalDevice.h"
#include "RenderSystem.h"
#include "RenderGraph/RenderGraphTypes.hpp"

BLAS::~BLAS()
{
	if (myAccelerationStructure)
	{
		VulkanContext::GetDevice().GetDevice().destroyAccelerationStructureKHR(myAccelerationStructure);
		myAccelerationStructure = nullptr;
	}

	if (myBLASBuffer)
	{
		VulkanAllocator::DestroyBuffer_TS(myBLASBuffer);
		myBLASBuffer = nullptr;
	}
}

BLAS* BLAS::Build(const List<glm::vec3>& inPositions, const List<uint>& inIndices)
{
	check(!inPositions.IsEmpty() && !inIndices.IsEmpty());

	const uint vertexCount = static_cast<uint>(inPositions.size());
	const uint indexCount  = static_cast<uint>(inIndices.size());
	const uint primitiveCount = indexCount / 3;

	// ---- Geometry input buffers (mappable host-visible, readable by the device) ----
	const uint vertexDataSize = vertexCount * sizeof(glm::vec3);
	const uint indexDataSize  = indexCount  * sizeof(uint);

	VulkanBuffer* vertexInputBuffer = VulkanAllocator::AllocateBuffer_TS(
		"BLAS_VertexInput",
		VulkanBuffer::BLASInputCreateInfo(vertexDataSize),
		VMA_MEMORY_USAGE_AUTO,
		true);
	vertexInputBuffer->SetData(inPositions.data(), vertexDataSize);

	VulkanBuffer* indexInputBuffer = VulkanAllocator::AllocateBuffer_TS(
		"BLAS_IndexInput",
		VulkanBuffer::BLASInputCreateInfo(indexDataSize),
		VMA_MEMORY_USAGE_AUTO,
		true);
	indexInputBuffer->SetData(inIndices.data(), indexDataSize);

	// ---- Device addresses for build inputs ----
	vk::Device device = VulkanContext::GetDevice().GetDevice();

	const VkDeviceAddress vertexAddress = device.getBufferAddress(
		vk::BufferDeviceAddressInfo().setBuffer(vertexInputBuffer->GetAPIResource()));
	const VkDeviceAddress indexAddress = device.getBufferAddress(
		vk::BufferDeviceAddressInfo().setBuffer(indexInputBuffer->GetAPIResource()));

	// ---- Geometry description ----
	vk::AccelerationStructureGeometryTrianglesDataKHR trianglesData{};
	trianglesData.setVertexFormat(vk::Format::eR32G32B32Sfloat);
	trianglesData.setVertexData(vk::DeviceOrHostAddressConstKHR().setDeviceAddress(vertexAddress));
	trianglesData.setVertexStride(sizeof(glm::vec3));
	trianglesData.setMaxVertex(vertexCount - 1);
	trianglesData.setIndexType(vk::IndexType::eUint32);
	trianglesData.setIndexData(vk::DeviceOrHostAddressConstKHR().setDeviceAddress(indexAddress));

	vk::AccelerationStructureGeometryKHR geometry{};
	geometry.setGeometryType(vk::GeometryTypeKHR::eTriangles);
	geometry.setGeometry(vk::AccelerationStructureGeometryDataKHR().setTriangles(trianglesData));
	geometry.setFlags(vk::GeometryFlagBitsKHR::eOpaque);

	// ---- Query build sizes ----
	vk::AccelerationStructureBuildGeometryInfoKHR buildInfo{};
	buildInfo.setType(vk::AccelerationStructureTypeKHR::eBottomLevel);
	buildInfo.setFlags(vk::BuildAccelerationStructureFlagBitsKHR::ePreferFastTrace);
	buildInfo.setMode(vk::BuildAccelerationStructureModeKHR::eBuild);
	buildInfo.setGeometries(geometry);

	vk::AccelerationStructureBuildSizesInfoKHR buildSizes{};
	device.getAccelerationStructureBuildSizesKHR(
		vk::AccelerationStructureBuildTypeKHR::eDevice,
		&buildInfo,
		&primitiveCount,
		&buildSizes);

	// ---- BLAS storage buffer ----
	VulkanBuffer* blasBuffer = VulkanAllocator::AllocateBuffer_TS(
		"BLAS_Storage",
		VulkanBuffer::BLASStorageCreateInfo(static_cast<uint>(buildSizes.accelerationStructureSize)),
		VMA_MEMORY_USAGE_AUTO,
		false);

	// ---- Create the acceleration structure ----
	vk::AccelerationStructureCreateInfoKHR asCreateInfo{};
	asCreateInfo.setBuffer(blasBuffer->GetAPIResource());
	asCreateInfo.setSize(buildSizes.accelerationStructureSize);
	asCreateInfo.setType(vk::AccelerationStructureTypeKHR::eBottomLevel);

	vk::AccelerationStructureKHR accelerationStructure = device.createAccelerationStructureKHR(asCreateInfo);

	// ---- Query required scratch alignment ----
	vk::PhysicalDeviceAccelerationStructurePropertiesKHR asProps{};
	vk::PhysicalDeviceProperties2 deviceProps2{};
	deviceProps2.pNext = &asProps;
	VulkanContext::GetPhysicalDevice().GetPhysicalDevice().getProperties2(&deviceProps2);
	const VkDeviceAddress scratchAlignment = asProps.minAccelerationStructureScratchOffsetAlignment;

	// Over-allocate by alignment so we can round up the device address.
	VulkanBuffer* scratchBuffer = VulkanAllocator::AllocateBuffer_TS(
		"BLAS_Scratch",
		VulkanBuffer::BLASScratchCreateInfo(static_cast<uint>(buildSizes.buildScratchSize) + static_cast<uint>(scratchAlignment)),
		VMA_MEMORY_USAGE_AUTO,
		false);

	VkDeviceAddress scratchAddress = device.getBufferAddress(
		vk::BufferDeviceAddressInfo().setBuffer(scratchBuffer->GetAPIResource()));
	scratchAddress = (scratchAddress + scratchAlignment - 1) & ~(scratchAlignment - 1);

	// ---- Build BLAS object early so we can register it as a resource dependency ----
	// Device address can be queried immediately (CPU-side call on an already-created handle).
	const VkDeviceAddress blasDeviceAddress = device.getAccelerationStructureAddressKHR(
		vk::AccelerationStructureDeviceAddressInfoKHR().setAccelerationStructure(accelerationStructure));

	BLAS* blas = new BLAS();
	blas->myAccelerationStructure = accelerationStructure;
	blas->myBLASBuffer = blasBuffer;
	blas->myDeviceAddress = blasDeviceAddress;

	// ---- Record build command ----
	buildInfo.setDstAccelerationStructure(accelerationStructure);
	buildInfo.setScratchData(vk::DeviceOrHostAddressKHR().setDeviceAddress(scratchAddress));

	vk::AccelerationStructureBuildRangeInfoKHR rangeInfo{};
	rangeInfo.setPrimitiveCount(primitiveCount);
	rangeInfo.setPrimitiveOffset(0);
	rangeInfo.setFirstVertex(0);
	rangeInfo.setTransformOffset(0);

	VulkanCommandBuffer* commandBuffer = RenderSystem::CreateUploadCommandBuffer_TS();
	commandBuffer->GetAPIResource().buildAccelerationStructuresKHR(buildInfo, &rangeInfo);

	// Declare the BLAS itself as an AS resource (not a buffer) so that the render graph
	// emits a proper global memory barrier (eAccelerationStructureWriteKHR) that TLAS
	// read dependencies can synchronize against.
	List<ResourceUsage> resourceUsages{};
	resourceUsages.Emplace().SetToAccelerationStructure(blas->GetAccelerationStructure(),
		vk::PipelineStageFlagBits::eAccelerationStructureBuildKHR,
		vk::AccessFlagBits::eAccelerationStructureWriteKHR);
	resourceUsages.Emplace().SetToBuffer(vertexInputBuffer,
		vk::PipelineStageFlagBits::eAccelerationStructureBuildKHR,
		vk::AccessFlagBits::eShaderRead);
	resourceUsages.Emplace().SetToBuffer(indexInputBuffer,
		vk::PipelineStageFlagBits::eAccelerationStructureBuildKHR,
		vk::AccessFlagBits::eShaderRead);

	RenderSystem::QueueCommandBufferForUpload_TS(commandBuffer, resourceUsages);

	// Scratch and geometry input buffers are only needed for the build — defer destruction.
	VulkanAllocator::DestroyBuffer_TS(scratchBuffer);
	VulkanAllocator::DestroyBuffer_TS(vertexInputBuffer);
	VulkanAllocator::DestroyBuffer_TS(indexInputBuffer);

	return blas;
}