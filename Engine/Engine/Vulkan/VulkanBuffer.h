#pragma once

class VulkanBuffer
{
public:
	operator vk::Buffer()
	{
		return myBuffer;
	}

	vk::Buffer operator->()
	{
		return myBuffer;
	}

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