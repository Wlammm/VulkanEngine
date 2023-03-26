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
	const vk::SurfaceKHR& GetSurface() const;

	uint GetFrameIndex() const;
	uint GetSwapChainIndex() const;
	uint GetFrameLag() const;

	uint GetWidth() const;
	uint GetHeight() const;

	uint GetMinImageCount() const;

	const vk::ImageView& GetImageView(const uint inIndex) const;

private:
	void CreateWindowSurface();
	void CreateSyncObjects();
	void CreateSwapChain();
	void CreateCommandPoolAndBuffers();
	void CreateRenderPass();
	void CreateFrameBuffers();

	void Init();
	void DestroySwapChainRelatedObjects();


	void Resize();

	int GetPresentQueueIndex() const;
	vk::SurfaceFormatKHR PickSurfaceFormat() const;


private:
	const VulkanDevice& myDevice;

	vk::SurfaceKHR myWindowSurface;
	vk::Format myFormat;

	const int myFrameLag = 3;
	List<vk::Fence> myFences;
	List<vk::Semaphore> myImageAcquiredSemaphores;
	List<vk::Semaphore> myDrawCompleteSemaphores;

	// ============ Swapchain image index does not necessarily match frame index. ============ 
	uint mySwapChainImageIndex = 0; // Use this only for images & image views
	uint myFrameIndex = 0; // Use this for everything else.

	vk::SwapchainKHR mySwapChain;

	uint mySwapChainWidth;
	uint mySwapChainHeight;

	uint myMinImageCount = 0;
	
	List<vk::Image> myImages;
	List<vk::ImageView> myImageViews;
	class VulkanDepthBuffer* myDepthBuffer = nullptr;

	vk::CommandPool myCommandPool;
	List<vk::CommandBuffer> myCommandBuffers;

	vk::RenderPass myRenderPass;

	List<vk::Framebuffer> myFrameBuffers;
};