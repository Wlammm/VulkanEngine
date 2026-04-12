#include "EnginePch.h"
#include "TLAS.h"

#include "Engine/Vulkan/VulkanAllocator.h"
#include "Engine/Vulkan/VulkanBuffer.h"
#include "Engine/Vulkan/VulkanCommandBuffer.h"
#include "Engine/Vulkan/VulkanContext.h"
#include "Engine/Vulkan/VulkanDevice.h"
#include "Engine/Vulkan/VulkanPhysicalDevice.h"
#include "RenderSystem.h"
#include "RenderGraph/RenderGraphTypes.hpp"

TLAS::~TLAS()
{
	DestroyResources();
}

void TLAS::Build(const List<VkAccelerationStructureInstanceKHR>& inInstances)
{
	check(!inInstances.IsEmpty());

	const uint instanceCount    = static_cast<uint>(inInstances.size());
	const uint instanceDataSize = instanceCount * sizeof(VkAccelerationStructureInstanceKHR);

	vk::Device device = VulkanContext::GetDevice().GetDevice();

	// ---- Update or allocate instance buffer ----
	if (instanceCount > myInstanceCapacity)
	{
		if (myInstanceBuffer)
		{
			VulkanAllocator::DestroyBuffer_TS(myInstanceBuffer);
			myInstanceBuffer = nullptr;
		}

		myInstanceBuffer = VulkanAllocator::AllocateBuffer_TS(
			"TLAS_InstanceBuffer",
			VulkanBuffer::BLASInputCreateInfo(instanceDataSize),
			VMA_MEMORY_USAGE_AUTO, true);
	}

	myInstanceBuffer->SetData(inInstances.data(), instanceDataSize);

	// ---- Build geometry info ----
	const VkDeviceAddress instanceAddress = device.getBufferAddress(
		vk::BufferDeviceAddressInfo().setBuffer(myInstanceBuffer->GetAPIResource()));

	vk::AccelerationStructureGeometryInstancesDataKHR instancesData{};
	instancesData.setArrayOfPointers(VK_FALSE);
	instancesData.setData(vk::DeviceOrHostAddressConstKHR().setDeviceAddress(instanceAddress));

	vk::AccelerationStructureGeometryKHR geometry{};
	geometry.setGeometryType(vk::GeometryTypeKHR::eInstances);
	geometry.setGeometry(vk::AccelerationStructureGeometryDataKHR().setInstances(instancesData));
	geometry.setFlags(vk::GeometryFlagBitsKHR::eOpaque);

	vk::AccelerationStructureBuildGeometryInfoKHR buildInfo{};
	buildInfo.setType(vk::AccelerationStructureTypeKHR::eTopLevel);
	buildInfo.setFlags(vk::BuildAccelerationStructureFlagBitsKHR::ePreferFastTrace);
	buildInfo.setMode(vk::BuildAccelerationStructureModeKHR::eBuild);
	buildInfo.setGeometries(geometry);

	// ---- Recreate storage + scratch if instance count grew ----
	if (instanceCount > myInstanceCapacity)
		Recreate(instanceCount, buildInfo);

	// ---- Align scratch address ----
	VkDeviceAddress scratchAddress = device.getBufferAddress(
		vk::BufferDeviceAddressInfo().setBuffer(myScratchBuffer->GetAPIResource()));
	scratchAddress = (scratchAddress + myScratchAlignment - 1) & ~(VkDeviceAddress)(myScratchAlignment - 1);

	buildInfo.setDstAccelerationStructure(myAS);
	buildInfo.setScratchData(vk::DeviceOrHostAddressKHR().setDeviceAddress(scratchAddress));

	vk::AccelerationStructureBuildRangeInfoKHR rangeInfo{};
	rangeInfo.setPrimitiveCount(instanceCount);

	// ---- Queue build command ----
	VulkanCommandBuffer* commandBuffer = RenderSystem::CreateUploadCommandBuffer_TS();
	commandBuffer->GetAPIResource().buildAccelerationStructuresKHR(buildInfo, &rangeInfo);

	List<ResourceUsage> resourceUsages{};
	resourceUsages.Emplace().SetToAccelerationStructure(
		myAS,
		vk::PipelineStageFlagBits::eAccelerationStructureBuildKHR,
		vk::AccessFlagBits::eAccelerationStructureWriteKHR);
	resourceUsages.Emplace().SetToBuffer(
		myInstanceBuffer,
		vk::PipelineStageFlagBits::eAccelerationStructureBuildKHR,
		vk::AccessFlagBits::eShaderRead);

	RenderSystem::QueueCommandBufferForUpload_TS(commandBuffer, resourceUsages);
}

void TLAS::Recreate(uint inInstanceCount, const vk::AccelerationStructureBuildGeometryInfoKHR& inBuildInfo)
{
	vk::Device device = VulkanContext::GetDevice().GetDevice();

	// ---- Query build sizes ----
	vk::AccelerationStructureBuildSizesInfoKHR buildSizes{};
	device.getAccelerationStructureBuildSizesKHR(
		vk::AccelerationStructureBuildTypeKHR::eDevice,
		&inBuildInfo,
		&inInstanceCount,
		&buildSizes);

	// ---- Destroy old AS + storage + scratch (deferred) ----
	if (myAS)
	{
		vk::AccelerationStructureKHR oldAS = myAS;
		VulkanAllocator::QueueDestroyCommand([oldAS]()
		{
			VulkanContext::GetDevice().GetDevice().destroyAccelerationStructureKHR(oldAS);
		});
		myAS = nullptr;
	}
	if (myASBuffer)    { VulkanAllocator::DestroyBuffer_TS(myASBuffer);    myASBuffer    = nullptr; }
	if (myScratchBuffer) { VulkanAllocator::DestroyBuffer_TS(myScratchBuffer); myScratchBuffer = nullptr; }

	// ---- Storage buffer + AS object ----
	myASBuffer = VulkanAllocator::AllocateBuffer_TS(
		"TLAS_Storage",
		VulkanBuffer::BLASStorageCreateInfo(static_cast<uint>(buildSizes.accelerationStructureSize)),
		VMA_MEMORY_USAGE_AUTO, false);

	vk::AccelerationStructureCreateInfoKHR asCreateInfo{};
	asCreateInfo.setBuffer(myASBuffer->GetAPIResource());
	asCreateInfo.setSize(buildSizes.accelerationStructureSize);
	asCreateInfo.setType(vk::AccelerationStructureTypeKHR::eTopLevel);
	myAS = device.createAccelerationStructureKHR(asCreateInfo);

	// ---- Query scratch alignment (device constant — only needed on first creation) ----
	if (myScratchAlignment == 128)
	{
		vk::PhysicalDeviceAccelerationStructurePropertiesKHR asProps{};
		vk::PhysicalDeviceProperties2 deviceProps2{};
		deviceProps2.pNext = &asProps;
		VulkanContext::GetPhysicalDevice().GetPhysicalDevice().getProperties2(&deviceProps2);
		myScratchAlignment = asProps.minAccelerationStructureScratchOffsetAlignment;
	}

	// ---- Scratch buffer ----
	const uint scratchSize = static_cast<uint>(buildSizes.buildScratchSize) + myScratchAlignment;
	myScratchBuffer = VulkanAllocator::AllocateBuffer_TS(
		"TLAS_Scratch",
		VulkanBuffer::BLASScratchCreateInfo(scratchSize),
		VMA_MEMORY_USAGE_AUTO, false);

	myInstanceCapacity = inInstanceCount;
	myDeviceAddress = device.getAccelerationStructureAddressKHR(
		vk::AccelerationStructureDeviceAddressInfoKHR().setAccelerationStructure(myAS));

	myOnRebuilt.Invoke();
}

void TLAS::DestroyResources()
{
	if (myAS)
	{
		vk::AccelerationStructureKHR as = myAS;
		VulkanAllocator::QueueDestroyCommand([as]()
		{
			VulkanContext::GetDevice().GetDevice().destroyAccelerationStructureKHR(as);
		});
		myAS = nullptr;
	}

	if (myASBuffer)      { VulkanAllocator::DestroyBuffer_TS(myASBuffer);      myASBuffer      = nullptr; }
	if (myInstanceBuffer){ VulkanAllocator::DestroyBuffer_TS(myInstanceBuffer); myInstanceBuffer = nullptr; }
	if (myScratchBuffer) { VulkanAllocator::DestroyBuffer_TS(myScratchBuffer);  myScratchBuffer  = nullptr; }

	myInstanceCapacity = 0;
	myDeviceAddress    = 0;
}