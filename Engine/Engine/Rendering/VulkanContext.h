#pragma once

class VulkanContext
{
public:
	VulkanContext();
	~VulkanContext();

private:
	void CheckValidationLayerSupport();
	void CheckExtensionSupport();

	void CreateInstance();
	void CreateDebugLayer();
	
	static VkBool32 DebugMessageCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData);

private:
	inline static VulkanContext* myInstance = nullptr;

	vk::Instance myVulkanInstance;
	vk::DebugUtilsMessengerEXT myDebugMessenger;

	const std::vector<const char*> myExtensions
	{
#ifdef DEBUG
		VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
#endif
	};

	const std::vector<const char*> myLayers
	{ 
#ifdef DEBUG
		"VK_LAYER_KHRONOS_validation",
#endif
	};
};