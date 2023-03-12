#pragma once

class VulkanContext
{
public:
	VulkanContext();
	~VulkanContext();

	static vk::Instance& GetInstance();
	static class VulkanPhysicalDevice& GetPhysicalDevice();
	static class VulkanDevice& GetDevice();
	static class VulkanSwapChain& GetSwapChain();
	static vk::PipelineCache& GetPipelineCache();
	static class VulkanAllocator& GetAllocator();

	static Vec2f GetRenderResolution();

	static void BeginFrame();
	static void EndFrame();

private:
	void CheckValidationLayerSupport();
	void CheckExtensionSupport();

	void CreateInstance();
	void CreateDebugLayer();
	void DestroyDebugLayer();

private:
	inline static VulkanContext* myInstance = nullptr;

	vk::Instance myVulkanInstance;
	vk::DebugUtilsMessengerEXT myDebugMessenger;
	vk::PipelineCache myPipelineCache;

	class VulkanPhysicalDevice* myPhysicalDevice = nullptr;
	class VulkanDevice* myDevice = nullptr;
	class VulkanSwapChain* mySwapChain = nullptr;
	class VulkanAllocator* myAllocator = nullptr;

	const List<const char*> myExtensions
	{
#ifdef DEBUG
		VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
#endif
		VK_KHR_SURFACE_EXTENSION_NAME,
		VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
	};

	const List<const char*> myLayers
	{ 
#ifdef DEBUG
		"VK_LAYER_KHRONOS_validation",
#endif
	};

private:
	static VkBool32 DebugMessageCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData);
};