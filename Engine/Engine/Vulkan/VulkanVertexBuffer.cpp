#include "EnginePch.h"
#include "VulkanVertexBuffer.h"
#include "VulkanContext.h"
#include "VulkanAllocator.h"
#include "VulkanDevice.h"

VulkanVertexBuffer::VulkanVertexBuffer(const List<Vertex>& inVertices)
{
	vk::BufferCreateInfo stagingCreateInfo = vk::BufferCreateInfo()
		.setSize(inVertices.size() * sizeof(Vertex))
		.setUsage(vk::BufferUsageFlagBits::eTransferSrc)
		.setSharingMode(vk::SharingMode::eExclusive);

	VulkanBuffer* stagingBuffer = VulkanContext::GetAllocator().AllocateBuffer("Vertex staging buffer", stagingCreateInfo, VMA_MEMORY_USAGE_CPU_TO_GPU);
	void* stagingMemory = stagingBuffer->Map();
	memcpy(stagingMemory, inVertices.data(), inVertices.size() * sizeof(Vertex));
	stagingBuffer->Unmap();

	vk::BufferCreateInfo createInfo = vk::BufferCreateInfo()
		.setSize(inVertices.size() * sizeof(Vertex))
		.setUsage(vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer);

	myBuffer = VulkanContext::GetAllocator().AllocateBuffer("VertexBuffer", createInfo, VMA_MEMORY_USAGE_GPU_ONLY);

	vk::CommandBuffer copyCmd = VulkanContext::GetDevice().CreateCommandBuffer(true);

	vk::BufferCopy copyRegion = vk::BufferCopy().setSize(sizeof(Vertex) * inVertices.size());
	copyCmd.copyBuffer(*stagingBuffer, *myBuffer, { copyRegion });

	VulkanContext::GetDevice().FlushCommandBuffer(copyCmd);
	VulkanContext::GetAllocator().DestroyBuffer(stagingBuffer);
}

VulkanVertexBuffer::~VulkanVertexBuffer()
{
	VulkanContext::GetAllocator().DestroyBuffer(myBuffer);
	myBuffer = nullptr;
}

void VulkanVertexBuffer::Bind(vk::CommandBuffer inCommandBuffer)
{
	inCommandBuffer.bindVertexBuffers(0, { *myBuffer }, { 0 });
}
