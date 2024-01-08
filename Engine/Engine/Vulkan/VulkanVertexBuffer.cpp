#include "EnginePch.h"
#include "VulkanVertexBuffer.h"
#include "VulkanContext.h"
#include "VulkanAllocator.h"
#include "VulkanDevice.h"
#include "VulkanBuffer.h"
#include "Tracy/tracy/Tracy.hpp"

VulkanVertexBuffer::VulkanVertexBuffer(const List<Vertex>& inVertices)
{
	ZoneScoped;
	vk::BufferCreateInfo stagingCreateInfo = vk::BufferCreateInfo()
		.setSize(inVertices.size() * sizeof(Vertex))
		.setUsage(vk::BufferUsageFlagBits::eTransferSrc)
		.setSharingMode(vk::SharingMode::eExclusive);

	VulkanBuffer* stagingBuffer = VulkanAllocator::AllocateBuffer_TS("Vertex staging buffer", stagingCreateInfo, VMA_MEMORY_USAGE_AUTO, true);
	void* stagingMemory = stagingBuffer->Map();
	memcpy(stagingMemory, inVertices.data(), inVertices.size() * sizeof(Vertex));
	stagingBuffer->Unmap();

	vk::BufferCreateInfo createInfo = vk::BufferCreateInfo()
		.setSize(inVertices.size() * sizeof(Vertex))
		.setUsage(vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer);

	myBuffer = VulkanAllocator::AllocateBuffer_TS("Vertex buffer", createInfo, VMA_MEMORY_USAGE_AUTO);

	vk::CommandBuffer copyCmd = VulkanContext::GetDevice().CreateCommandBuffer(true);

	vk::BufferCopy copyRegion = vk::BufferCopy().setSize(sizeof(Vertex) * inVertices.size());
	copyCmd.copyBuffer(stagingBuffer->GetAPIResource(), myBuffer->GetAPIResource(), { copyRegion });

	VulkanContext::GetDevice().FlushCommandBuffer(copyCmd);
	VulkanAllocator::DestroyBuffer_TS(stagingBuffer);
}

VulkanVertexBuffer::~VulkanVertexBuffer()
{
	VulkanAllocator::DestroyBuffer_TS(myBuffer);
	myBuffer = nullptr;
}

void VulkanVertexBuffer::Bind(vk::CommandBuffer inCommandBuffer) const
{
	inCommandBuffer.bindVertexBuffers(0, { myBuffer->GetAPIResource() }, { 0 });
}
