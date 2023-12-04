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
	friend class VulkanAllocator;
	VmaAllocation myAllocation;

	vk::Buffer myBuffer;

#ifdef DEBUG
	std::string myName = "";
#endif
};