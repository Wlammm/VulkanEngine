#pragma once

class VulkanAllocator
{
public:
	VulkanAllocator(vk::Instance inInstance, const class VulkanPhysicalDevice& inPhysicalDevice, const class VulkanDevice& inDevice);
	~VulkanAllocator();

	void Tick();

	static class VulkanBuffer* AllocateBuffer_TS(const std::string& inName, const vk::BufferCreateInfo& inCreateInfo, VmaMemoryUsage inUsage, bool inMappable = false);
	static void DestroyBuffer_TS(class VulkanBuffer* inBuffer);
	static void DestroyBuffer_TS(class ResizableBuffer* inBuffer);

	static class VulkanImage* AllocateImage_TS(const std::string& inName, const vk::ImageCreateInfo& inCreateInfo, VmaMemoryUsage inUsage);
	static void DestroyImage_TS(class VulkanImage* inImage);

	static VmaAllocator GetVMAAllocator();

private:
	void DestroyBufferInternal(VulkanBuffer* inBuffer);
	void DestroyImageInternal(VulkanImage* inImage);

	void TickBufferDeletes();
	void TickImageDeletes();

private:
	inline static VulkanAllocator* myInstance = nullptr;

	VmaAllocator myAllocator;

	template<typename T>
	struct DeleteData
	{
		int myFramesUntilDelete = 0;
		T* myData;
	};
	MutexList<DeleteData<VulkanBuffer>> myBufferDeleteData;
	MutexList<DeleteData<ResizableBuffer>> myResizableBufferDeleteData;
	MutexList<DeleteData<VulkanImage>> myImageDeleteData;

#if DEBUG
	MutexList<std::string> myAllocatedNames;
#endif
};