#pragma once

class VulkanAllocator
{
public:
	VulkanAllocator(vk::Instance inInstance, const class VulkanPhysicalDevice& inPhysicalDevice, const class VulkanDevice& inDevice);
	~VulkanAllocator();

	static class VulkanBuffer* AllocateBuffer_TS(const std::string& inName, const vk::BufferCreateInfo& inCreateInfo, VmaMemoryUsage inUsage);
	static void DestroyBuffer_TS(class VulkanBuffer* inBuffer);

	static class VulkanImage* AllocateImage_TS(const std::string& inName, const vk::ImageCreateInfo& inCreateInfo, VmaMemoryUsage inUsage);
	static void DestroyImage_TS(class VulkanImage* inImage);

	operator VmaAllocator()
	{
		return myAllocator;
	}
	
private:
	inline static VulkanAllocator* myInstance = nullptr;

	VmaAllocator myAllocator;

#if DEBUG
	std::mutex myAllocatedNamesMutex;
	List<std::string> myAllocatedNames;
#endif
};