#pragma once

class VulkanDevice;

class VulkanSwapChain
{
public:
	VulkanSwapChain(const VulkanDevice& inDevice);
	~VulkanSwapChain();

private:
	void CreateWindowSurface();
	void CreateSyncObjects();
	void CreateSwapChain();

	int GetPresentQueueIndex() const;
	vk::SurfaceFormatKHR PickSurfaceFormat() const;


private:
	const VulkanDevice& myDevice;

	vk::SurfaceKHR myWindowSurface;

	const int myFrameLag = 3;
	std::vector<vk::Fence> myFences;
	std::vector<vk::Semaphore> myImageAcquiredSemaphores;
	std::vector<vk::Semaphore> myDrawCompleteSemaphores;

	int myFrameIndex = 0;

	vk::SwapchainKHR mySwapChain;

	uint mySwapChainWidth;
	uint mySwapChainHeight;
	
	std::vector<vk::Image> myImages;
	std::vector<vk::ImageView> myImageViews;
};