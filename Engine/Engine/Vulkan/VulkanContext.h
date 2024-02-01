#pragma once

class VulkanContext
{
public:
	VulkanContext();
	~VulkanContext();

	inline static constexpr int FrameLag = 3;

	static vk::Instance& GetInstance();
	static class VulkanPhysicalDevice& GetPhysicalDevice();
	static class VulkanDevice& GetDevice();
	static class VulkanSwapChain& GetSwapChain();
	static vk::PipelineCache& GetPipelineCache();
	static class VulkanAllocator& GetAllocator();

	static Vec2f GetRenderResolution();

	void BeginFrame();
	void EndFrame();

	template<typename T>
	static uint64_t GetVulkanHandle(T const& inCppHandle)
	{
		return uint64_t(static_cast<T::CType>(inCppHandle));
	}

	static vk::DescriptorPool GetDescriptorPool();

private:
	void CheckValidationLayerSupport();
	void CheckExtensionSupport();

	void CreateInstance();
	void CreateDebugLayer();
	void DestroyDebugLayer();

	void CreateDescriptorPool();

private:
	inline static VulkanContext* myInstance = nullptr;

	vk::Instance myVulkanInstance;
	vk::DebugUtilsMessengerEXT myDebugMessenger;
	vk::PipelineCache myPipelineCache;

	class VulkanPhysicalDevice* myPhysicalDevice = nullptr;
	class VulkanDevice* myDevice = nullptr;
	class VulkanAllocator* myAllocator = nullptr;
	class VulkanSwapChain* mySwapChain = nullptr;

	vk::DescriptorPool myDescriptorPool;

	const List<const char*> myExtensions
	{
#ifdef DEBUG
		VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
#endif
		VK_KHR_SURFACE_EXTENSION_NAME,
		VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
		VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME,
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