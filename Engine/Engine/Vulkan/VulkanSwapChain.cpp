#include "EnginePch.h"
#include "VulkanSwapChain.h"
#include "Windows/WindowHandler.h"
#include "VulkanContext.h"
#include "VulkanPhysicalDevice.h"
#include "VulkanDevice.h"
#include "Engine.h"
#include "VulkanDepthBuffer.h"
#include "ECS/Systems/RenderSystem.h"
#include "Events/EventHandler.h"

VulkanSwapChain::VulkanSwapChain(const VulkanDevice& inDevice)
	: myDevice{ inDevice }
{
	CreateWindowSurface();
	Init();

	LOG("Swapchain created.");
}

VulkanSwapChain::~VulkanSwapChain()
{
	myDevice->waitIdle();
	DestroySwapChainRelatedObjects();

	myDevice->destroySwapchainKHR(mySwapChain);
	VulkanContext::GetInstance().destroySurfaceKHR(myWindowSurface);
}

void VulkanSwapChain::BeginFrame()
{
	// Wait for gpu to finish.
	vk::Result result = myDevice->waitForFences(myFences[myFrameIndex], VK_TRUE, UINT64_MAX);
	check(result == vk::Result::eSuccess);

	myDevice->resetFences({ myFences[myFrameIndex] });

	do
	{
		result = myDevice->acquireNextImageKHR(mySwapChain, UINT64_MAX, myImageAcquiredSemaphores[myFrameIndex], vk::Fence(), &myFrameIndex);

		if(result == vk::Result::eErrorOutOfDateKHR)
		{
			Resize();
		}
		else if(result == vk::Result::eSuboptimalKHR)
		{
			LOG("Swapchain is suboptimal.");
			break;
		}
		else if(result == vk::Result::eErrorSurfaceLostKHR)
		{
			LOG("Swapchain lost window surface. Recreating..");
			VulkanContext::GetInstance().destroySurfaceKHR(myWindowSurface);
			CreateWindowSurface();
			Resize();
		}
		else
		{
			THROW_IF(result != vk::Result::eSuccess, "Swapchain error cannot be handled.");
		}
	} while (result != vk::Result::eSuccess);
}

void VulkanSwapChain::EndFrame()
{
	vk::PipelineStageFlags pipelineStageFlags = vk::PipelineStageFlagBits::eColorAttachmentOutput;

	VulkanContext::GetDevice().GetGraphicsQueue().submit(vk::SubmitInfo()
		.setWaitDstStageMask(pipelineStageFlags)
		.setWaitSemaphores(myImageAcquiredSemaphores[myFrameIndex])
		.setCommandBuffers(GetCommandBuffer())
		.setSignalSemaphores(myDrawCompleteSemaphores[myFrameIndex]),
		myFences[myFrameIndex]);

	const vk::PresentInfoKHR presentInfo = vk::PresentInfoKHR()
		.setWaitSemaphores(myDrawCompleteSemaphores[myFrameIndex])
		.setSwapchains(mySwapChain)
		.setImageIndices(myFrameIndex);

	vk::Result result = VulkanContext::GetDevice().GetPresentQueue().presentKHR(&presentInfo);

	if(result == vk::Result::eErrorOutOfDateKHR)
	{
		Resize();
	}
	else if(result == vk::Result::eSuboptimalKHR)
	{
		vk::SurfaceCapabilitiesKHR surfCapabilities;
		auto caps_result = VulkanContext::GetPhysicalDevice()->getSurfaceCapabilitiesKHR(myWindowSurface, &surfCapabilities);
		check(caps_result == vk::Result::eSuccess);
		if (surfCapabilities.currentExtent.width != mySwapChainWidth || surfCapabilities.currentExtent.height != mySwapChainHeight)
		{
			Resize();
		}
	}
	else if(result == vk::Result::eErrorSurfaceLostKHR)
	{
		VulkanContext::GetInstance().destroySurfaceKHR(myWindowSurface);
		CreateWindowSurface();
		Resize();
	}
	else
	{
		check(result == vk::Result::eSuccess);
	}
	myFrameIndex += 1;
	myFrameIndex %= VulkanContext::FrameLag;
}

const vk::CommandBuffer& VulkanSwapChain::GetCommandBuffer() const
{
	return myCommandBuffers[myFrameIndex];
}

const vk::Image& VulkanSwapChain::GetImage() const
{
	return myImages[myFrameIndex];
}

const vk::SurfaceKHR& VulkanSwapChain::GetSurface() const
{
	return myWindowSurface;
}

const vk::Format& VulkanSwapChain::GetFormat() const
{
	return myFormat;
}

uint VulkanSwapChain::GetFrameIndex() const
{
	return myFrameIndex;
}

uint VulkanSwapChain::GetWidth() const
{
	return mySwapChainWidth;
}

uint VulkanSwapChain::GetHeight() const
{
	return mySwapChainHeight;
}

uint VulkanSwapChain::GetMinImageCount() const
{
	return myMinImageCount;
}

const vk::ImageView& VulkanSwapChain::GetImageView(const uint inIndex) const
{
	return myImageViews[inIndex];
}

void VulkanSwapChain::CreateWindowSurface()
{
	vk::Win32SurfaceCreateInfoKHR createInfo = vk::Win32SurfaceCreateInfoKHR().setHinstance(WindowHandler::GetHInstance()).setHwnd(WindowHandler::GetHWND());
	vk::Result result = VulkanContext::GetInstance().createWin32SurfaceKHR(&createInfo, nullptr, &myWindowSurface);
	check(result == vk::Result::eSuccess);
}

void VulkanSwapChain::CreateSyncObjects()
{
	vk::FenceCreateInfo fenceCreateInfo = vk::FenceCreateInfo().setFlags(vk::FenceCreateFlagBits::eSignaled);
	vk::SemaphoreCreateInfo semaCreateInfo = vk::SemaphoreCreateInfo();

	for(int i = 0; i < VulkanContext::FrameLag; ++i)
	{
		myFences.Add(myDevice->createFence(fenceCreateInfo));
		myImageAcquiredSemaphores.Add(myDevice->createSemaphore(semaCreateInfo));
		myDrawCompleteSemaphores.Add(myDevice->createSemaphore(semaCreateInfo));
	}
}

void VulkanSwapChain::CreateSwapChain()
{
	vk::SwapchainKHR oldSwapChain = mySwapChain;

	const int presentQueueIndex = GetPresentQueueIndex();
	const vk::SurfaceFormatKHR imageFormat = PickSurfaceFormat();
	myFormat = imageFormat.format;

	vk::SurfaceCapabilitiesKHR surfaceCapabilities = VulkanContext::GetPhysicalDevice()->getSurfaceCapabilitiesKHR(myWindowSurface);

	vk::Extent2D swapchainExtent;
	if(surfaceCapabilities.currentExtent.width == (uint32_t)-1)
	{
		mySwapChainWidth = swapchainExtent.width = Engine::GetEngineProperties().WindowWidth;
		mySwapChainHeight = swapchainExtent.height = Engine::GetEngineProperties().WindowHeight;
	}
	else
	{
		swapchainExtent = surfaceCapabilities.currentExtent;
		mySwapChainWidth = surfaceCapabilities.currentExtent.width;
		mySwapChainHeight = surfaceCapabilities.currentExtent.height;
	}

	// Disable vsync if possible. The only mode that is required to be implemented is fifo so use as fallback in-case immediate does not exists.
	vk::PresentModeKHR swapChainPresentMode = vk::PresentModeKHR::eFifo;
	List<vk::PresentModeKHR> availablePresentModes = VulkanContext::GetPhysicalDevice()->getSurfacePresentModesKHR(myWindowSurface);
	for(const auto& mode : availablePresentModes)
	{
		if (mode == vk::PresentModeKHR::eImmediate)
			swapChainPresentMode = vk::PresentModeKHR::eImmediate;
	}

	uint32_t numSwapchainImages = 3;

	myMinImageCount = surfaceCapabilities.minImageCount;

	if (numSwapchainImages < surfaceCapabilities.minImageCount)
		numSwapchainImages = surfaceCapabilities.minImageCount;

	if (surfaceCapabilities.maxImageCount > 0 && numSwapchainImages > surfaceCapabilities.maxImageCount)
		numSwapchainImages = surfaceCapabilities.maxImageCount;

	vk::SurfaceTransformFlagBitsKHR preTransform;
	if (surfaceCapabilities.supportedTransforms & vk::SurfaceTransformFlagBitsKHR::eIdentity)
		preTransform = vk::SurfaceTransformFlagBitsKHR::eIdentity;
	else
		preTransform = surfaceCapabilities.currentTransform;

	// One of these flags are guaranteed to be set. Pick one and use.
	const std::array<vk::CompositeAlphaFlagBitsKHR, 4> compositeAlphaFlags = {
		vk::CompositeAlphaFlagBitsKHR::eOpaque,
		vk::CompositeAlphaFlagBitsKHR::ePreMultiplied,
		vk::CompositeAlphaFlagBitsKHR::ePostMultiplied,
		vk::CompositeAlphaFlagBitsKHR::eInherit,
	};

	vk::CompositeAlphaFlagBitsKHR compositeAlpha;
	for(const auto& alphaFlag : compositeAlphaFlags)
	{
		if (surfaceCapabilities.supportedCompositeAlpha & alphaFlag)
			compositeAlpha = alphaFlag;
	}

	mySwapChain = myDevice->createSwapchainKHR(vk::SwapchainCreateInfoKHR()
		.setSurface(myWindowSurface)
		.setMinImageCount(numSwapchainImages)
		.setImageFormat(imageFormat.format)
		.setImageColorSpace(imageFormat.colorSpace)
		.setImageExtent(swapchainExtent)
		.setImageArrayLayers(1)
		.setImageUsage(vk::ImageUsageFlagBits::eColorAttachment)
		.setImageSharingMode(vk::SharingMode::eExclusive)
		.setPreTransform(preTransform)
		.setCompositeAlpha(compositeAlpha)
		.setPresentMode(swapChainPresentMode)
		.setClipped(true)
		.setOldSwapchain(oldSwapChain)
	);

	if (oldSwapChain)
		myDevice->destroySwapchainKHR(oldSwapChain);

	myImages = myDevice->getSwapchainImagesKHR(mySwapChain);
	for(int i = 0; i < myImages.size(); ++i)
	{
		vk::ImageViewCreateInfo imageViewCreateInfo = vk::ImageViewCreateInfo()
			.setViewType(vk::ImageViewType::e2D)
			.setFormat(imageFormat.format)
			.setSubresourceRange(vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1));
		imageViewCreateInfo.image = myImages[i];

		myImageViews.Add(myDevice->createImageView(imageViewCreateInfo));
	}
}

void VulkanSwapChain::CreateCommandPoolAndBuffers()
{
	myCommandPool = myDevice->createCommandPool(vk::CommandPoolCreateInfo().setQueueFamilyIndex(VulkanContext::GetPhysicalDevice().GetGraphicsQueueIndex()).setFlags(vk::CommandPoolCreateFlags(vk::CommandPoolCreateFlagBits::eResetCommandBuffer)));
	
	myCommandBuffers = myDevice->allocateCommandBuffers(vk::CommandBufferAllocateInfo()
		.setCommandPool(myCommandPool)
		.setLevel(vk::CommandBufferLevel::ePrimary)
		.setCommandBufferCount(VulkanContext::FrameLag));
}

void VulkanSwapChain::Init()
{
	CreateSyncObjects();
	CreateSwapChain();
	CreateCommandPoolAndBuffers();

	myFrameIndex = 0;
	myFrameIndex = 0;
}

void VulkanSwapChain::DestroySwapChainRelatedObjects()
{
	for (int i = 0; i < VulkanContext::FrameLag; ++i)
	{
		myDevice->destroyFence(myFences[i]);
		myDevice->destroySemaphore(myImageAcquiredSemaphores[i]);
		myDevice->destroySemaphore(myDrawCompleteSemaphores[i]);
	}
	myFences.Clear();
	myImageAcquiredSemaphores.Clear();
	myDrawCompleteSemaphores.Clear();

	for (int i = 0; i < myImageViews.size(); ++i)
	{
		myDevice->destroyImageView(myImageViews[i]);
	}

	myDevice->freeCommandBuffers(myCommandPool, myCommandBuffers);
	myCommandBuffers.Clear();

	myDevice->destroyCommandPool(myCommandPool);

	myImageViews.Clear();
	myImages.Clear();

	myFrameIndex = 0;
	myFrameIndex = 0;
}

void VulkanSwapChain::Resize()
{
	myDevice->waitIdle();
	DestroySwapChainRelatedObjects();
	Init();

	Engine::GetEventHandler().FireEvent(EventType::SwapchainResized);

	LOG("Resize")
}

int VulkanSwapChain::GetPresentQueueIndex() const
{
	if (VulkanContext::GetPhysicalDevice()->getSurfaceSupportKHR(VulkanContext::GetPhysicalDevice().GetGraphicsQueueIndex(), myWindowSurface))
		return VulkanContext::GetPhysicalDevice().GetGraphicsQueueIndex();

	THROW("Separate present queues are not supported at the moment.");
	/*const auto& queueProperties = VulkanContext::GetPhysicalDevice().GetQueueFamilyProperties();

	for(int i  = 0; i < queueProperties.size(); ++i)
	{
		uint32_t supports = VulkanContext::GetPhysicalDevice()->getSurfaceSupportKHR(i, myWindowSurface);
		if(supports)
			return i;
	}

	THROW("Failed to find present queue index.");
	return -1;*/
}

vk::SurfaceFormatKHR VulkanSwapChain::PickSurfaceFormat() const
{
	auto availableFormats = VulkanContext::GetPhysicalDevice()->getSurfaceFormatsKHR(myWindowSurface);

	for(const auto& entry : availableFormats)
	{
		const vk::Format& format = entry.format;

		if(format == vk::Format::eR8G8B8A8Unorm || format == vk::Format::eB8G8R8A8Unorm ||
			format == vk::Format::eA2B10G10R10UnormPack32 || format == vk::Format::eA2R10G10B10UnormPack32 ||
			format == vk::Format::eR16G16B16A16Sfloat)
		{
			return entry;
		}
	}

	check(false && "Failed to find preferable surface format. Fallback to first format. Rendering may be incorrect.");
	THROW_IF(availableFormats.empty(), "No surface formats exists.");

	return availableFormats[0];
}
