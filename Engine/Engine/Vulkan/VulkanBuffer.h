#pragma once

class VulkanBuffer
{
public:
	vk::Buffer GetAPIResource();

	void* Map();
	void Unmap();

private:
	// Only create and destroy this resource via VulkanAllocator.
	VulkanBuffer() = default;
	~VulkanBuffer() = default;
	VulkanBuffer(const VulkanBuffer& inOther) = delete;

private:
	friend class VulkanAllocator;
	VmaAllocation myAllocation;

	vk::Buffer myBuffer;

#ifdef DEBUG
	std::string myName = "";
#endif
};