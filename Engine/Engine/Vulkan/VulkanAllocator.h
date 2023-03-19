#pragma once

class VulkanAllocator
{
public:
	VulkanAllocator(vk::Instance inInstance, const class VulkanPhysicalDevice& inPhysicalDevice, const class VulkanDevice& inDevice);
	~VulkanAllocator();

	class VulkanBuffer* AllocateBuffer(const std::string& inName, const vk::BufferCreateInfo& inCreateInfo, VmaMemoryUsage inUsage);
	void DestroyBuffer(class VulkanBuffer* inBuffer);

	class VulkanImage* AllocateImage(const std::string& inName, const vk::ImageCreateInfo& inCreateInfo, VmaMemoryUsage inUsage);
	void DestroyImage(class VulkanImage* inImage);

	operator VmaAllocator()
	{
		return myAllocator;
	}
	
private:
	VmaAllocator myAllocator;

#ifdef DEBUG
	List<class VulkanBuffer*> myBuffers{};
	List<class VulkanImage*> myImages{};
#endif
};