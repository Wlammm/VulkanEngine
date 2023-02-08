#pragma once

class VulkanDevice;

class VulkanSwapChain
{
public:
	VulkanSwapChain(const VulkanDevice& inDevice);
	~VulkanSwapChain();

	void BeginFrame();
	void EndFrame();

	const vk::CommandBuffer& GetCommandBuffer() const;
	const vk::Image& GetImage() const;
	const vk::RenderPass& GetRenderPass() const;
	const vk::Framebuffer& GetFrameBuffer() const;

private:
	void CreateWindowSurface();
	void CreateSyncObjects();
	void CreateSwapChain();
	void CreateCommandPoolAndBuffers();
	void CreateRenderPass();
	void CreateFrameBuffers();

	void Resize();

	int GetPresentQueueIndex() const;
	vk::SurfaceFormatKHR PickSurfaceFormat() const;


private:
	const VulkanDevice& myDevice;

	vk::SurfaceKHR myWindowSurface;
	vk::Format myFormat;

	const int myFrameLag = 3;
	std::vector<vk::Fence> myFences;
	std::vector<vk::Semaphore> myImageAcquiredSemaphores;
	std::vector<vk::Semaphore> myDrawCompleteSemaphores;

	// ============ Swapchain image index does not necessarily match frame index. ============ 
	uint mySwapChainImageIndex = 0; // Use this only for images & image views
	uint myFrameIndex = 0; // Use this for everything else.

	vk::SwapchainKHR mySwapChain;

	uint mySwapChainWidth;
	uint mySwapChainHeight;
	
	std::vector<vk::Image> myImages;
	std::vector<vk::ImageView> myImageViews;

	vk::CommandPool myCommandPool;
	std::vector<vk::CommandBuffer> myCommandBuffers;

	vk::RenderPass myRenderPass;

	std::vector<vk::Framebuffer> myFrameBuffers;
};