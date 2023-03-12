#pragma once

class VulkanIndexBuffer
{
public:
	VulkanIndexBuffer() = delete;
	VulkanIndexBuffer(const List<uint>& inIndices);
	~VulkanIndexBuffer();

	void Bind(vk::CommandBuffer inCommandBuffer);

private:
	class VulkanBuffer* myBuffer;
};