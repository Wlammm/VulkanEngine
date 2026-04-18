#pragma once

#include "Aftermath/NvidiaAftermathTracker.h"

class VulkanContext
{
public:
	VulkanContext();
	~VulkanContext();

	inline static constexpr int FrameLag = 3;
	inline static constexpr uint32_t AftermathMarkerHistory = c_markerFrameHistory;

	static vk::Instance& GetInstance();
	static class VulkanPhysicalDevice& GetPhysicalDevice();
	static class VulkanDevice& GetDevice();
	static class VulkanSwapChain& GetSwapChain();
	static vk::PipelineCache& GetPipelineCache();
	static class VulkanAllocator& GetAllocator();
	static class NvidiaAftermathTracker* GetAftermathTracker();
	static bool IsAftermathEnabled() { return GetAftermathTracker() != nullptr; }

	// Inserts an NVIDIA diagnostic checkpoint whose contents are resolved via the
	// app-managed marker callback (so crash dumps show the string).
	// Only active when `-aftermath` is enabled.
	static void SetAftermathCheckpoint(vk::CommandBuffer inCommandBuffer, std::string_view inLabel);

	static glm::vec2 GetRenderResolution();

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
	void LoadAccelerationStructureFunctions();

private:
	inline static VulkanContext* myInstance = nullptr;

	vk::Instance myVulkanInstance;
	vk::DebugUtilsMessengerEXT myDebugMessenger;
	vk::PipelineCache myPipelineCache;

	UniquePtr<class VulkanPhysicalDevice> myPhysicalDevice = nullptr;
	UniquePtr<class VulkanDevice> myDevice = nullptr;
	UniquePtr<class VulkanAllocator> myAllocator = nullptr;
	UniquePtr<class VulkanSwapChain> mySwapChain = nullptr;
	
	AfterMathMarkerMap markerMap;
	std::atomic<uint64_t> myAftermathMarkerId = 1;
	UniquePtr<class NvidiaAftermathTracker> myNvidiaAftermathDebugger = nullptr;

	vk::DescriptorPool myDescriptorPool;

	const List<const char*> myExtensions
	{
#if !SHIPPING
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
	static vk::Bool32 DebugMessageCallback(
		vk::DebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		vk::DebugUtilsMessageTypeFlagsEXT messageType,
		const vk::DebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData);
};