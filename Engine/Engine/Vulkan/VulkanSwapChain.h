#pragma once
#include "Engine/Delegates/MulticastDelegate.hpp"

class VulkanImage;
class VulkanDevice;

class VulkanSwapChain
{
public:
	VulkanSwapChain(const VulkanDevice& inDevice);
	~VulkanSwapChain();

	void BeginFrame();
	void EndFrame();

	const vk::CommandBuffer& GetCommandBuffer() const;
	const vk::SurfaceKHR& GetSurface() const;
	const vk::Format& GetFormat() const;

	uint GetFrameIndex() const;
	uint GetSyncIndex() const;

	uint GetWidth() const;
	uint GetHeight() const;

	uint GetMinImageCount() const;

	vk::ImageView GetImageView(const uint inIndex) const;
	VulkanImage* GetImage(const uint inIndex) const;
	VulkanImage* GetImage() const;

	inline static MulticastDelegate<void()> OnSwapChainResized;

private:
	void CreateWindowSurface();
	void CreateSyncObjects();
	void CreateSwapChain();
	void CreateCommandPoolAndBuffers();

	void Init();
	void DestroySwapChainRelatedObjects();


	void Resize();

	int GetPresentQueueIndex() const;
	vk::SurfaceFormatKHR PickSurfaceFormat() const;

private:
	const VulkanDevice& myDevice;

	vk::SurfaceKHR myWindowSurface;
	vk::Format myFormat;

	List<vk::Fence> myFences{};
	List<vk::Semaphore> myImageAcquiredSemaphores{};
	List<vk::Semaphore> myDrawCompleteSemaphores{};

	uint myFrameIndex = 0; 
	// We need a unique index for fences & semaphores that handles gpu waiting, since we need to wait for the correct fence before retreiving a new FrameIndex.
	uint mySyncIndex = 0;

	vk::SwapchainKHR mySwapChain;

	uint mySwapChainWidth = 0;
	uint mySwapChainHeight = 0;

	uint myMinImageCount = 0;
	
	List<VulkanImage*> myImages; 
	// List<vk::Image> myImages;
	// List<vk::ImageView> myImageViews;

	vk::CommandPool myCommandPool;
	List<vk::CommandBuffer> myCommandBuffers{};
};