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

	vk::Buffer myBuffer;
	VmaAllocation myAllocation;

#ifdef DEBUG
	std::string myName = "";
#endif
};