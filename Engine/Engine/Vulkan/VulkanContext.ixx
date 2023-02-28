module;

#include "Math/Vector2.hpp"
#include "Containers/List.hpp"

export module VulkanContext;
import <Vulkan/VulkanHpp.hpp>;

export class VulkanContext
{
public:
	VulkanContext();
	~VulkanContext();
	static vk::Instance& GetInstance();
	static class VulkanPhysicalDevice& GetPhysicalDevice();
	static class VulkanDevice& GetDevice();
	static class VulkanSwapChain& GetSwapChain();
	static vk::PipelineCache& GetPipelineCache();

	static Vec2f GetRenderResolution();

	static void BeginFrame();
	static void EndFrame();

private:
	void CheckValidationLayerSupport();
	void CheckExtensionSupport();

	void CreateInstance();
	void CreateDebugLayer();

private:
	inline static VulkanContext* myInstance = nullptr;

	vk::DynamicLoader myDynamicLoader;
	vk::Instance myVulkanInstance;
	vk::DebugUtilsMessengerEXT myDebugMessenger;
	vk::PipelineCache myPipelineCache;

	class VulkanPhysicalDevice* myPhysicalDevice = nullptr;
	class VulkanDevice* myDevice = nullptr;
	class VulkanSwapChain* mySwapChain = nullptr;

	const List<const char*> myExtensions
	{
#ifdef DEBUG
		VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
#endif
		VK_KHR_SURFACE_EXTENSION_NAME,
		"VK_KHR_win32_surface",
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