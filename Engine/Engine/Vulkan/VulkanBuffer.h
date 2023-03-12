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

protected:
	vk::Buffer myBuffer;

private:
	friend class VulkanAllocator;

	VmaAllocation myAllocation;

#ifdef DEBUG
	std::string myName = "";
#endif
};