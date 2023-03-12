#pragma once

class VulkanAllocator
{
public:
	VulkanAllocator(vk::Instance inInstance, const class VulkanPhysicalDevice& inPhysicalDevice, const class VulkanDevice& inDevice);
	~VulkanAllocator();

	class VulkanBuffer* AllocateBuffer(const std::string& inName, const vk::BufferCreateInfo& inCreateInfo, VmaMemoryUsage inUsage);
	void DestroyBuffer(VulkanBuffer* inBuffer);

	operator VmaAllocator()
	{
		return myAllocator;
	}
	
private:
	VmaAllocator myAllocator;

#ifdef DEBUG
	List<VulkanBuffer*> myBuffers{};
#endif
};