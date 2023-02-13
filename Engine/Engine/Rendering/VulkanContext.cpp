#include "EnginePch.h"
#include "VulkanContext.h"
#include "Engine.h"
#include "VulkanPhysicalDevice.h"
#include "VulkanDevice.h"
#include "VulkanSwapchain.h"
#include "Utils/String.hpp"

PFN_vkCreateDebugUtilsMessengerEXT pfnVkCreateDebugUtilsMessengerEXT;
PFN_vkDestroyDebugUtilsMessengerEXT pfnVkDestroyDebugUtilsMessengerEXT;

VKAPI_ATTR VkResult VKAPI_CALL vkCreateDebugUtilsMessengerEXT(VkInstance instance,
	const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
	const VkAllocationCallbacks* pAllocator,
	VkDebugUtilsMessengerEXT* pMessenger)
{
	return pfnVkCreateDebugUtilsMessengerEXT(instance, pCreateInfo, pAllocator, pMessenger);
}

VKAPI_ATTR void VKAPI_CALL vkDestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT messenger,
	VkAllocationCallbacks const* pAllocator)
{
	return pfnVkDestroyDebugUtilsMessengerEXT(instance, messenger, pAllocator);
}

VulkanContext::VulkanContext()
{
	check(!myInstance && "There can only be one vulkan context at the same time.");
	myInstance = this;

	CheckValidationLayerSupport();
	CheckExtensionSupport();
	CreateInstance();
	CreateDebugLayer();

	myPhysicalDevice = new VulkanPhysicalDevice();
	myDevice = new VulkanDevice(*myPhysicalDevice);
	mySwapChain = new VulkanSwapChain(*myDevice);

	myPipelineCache = GetDevice()->createPipelineCache(vk::PipelineCacheCreateInfo(), nullptr);
}

VulkanContext::~VulkanContext()
{
	GetDevice()->destroyPipelineCache(myPipelineCache);

	del(mySwapChain);
	del(myDevice);
	del(myPhysicalDevice);
	myVulkanInstance.destroyDebugUtilsMessengerEXT(myDebugMessenger);
	myVulkanInstance.destroy();
}

vk::Instance& VulkanContext::GetInstance()
{
	return myInstance->myVulkanInstance;
}

VulkanPhysicalDevice& VulkanContext::GetPhysicalDevice()
{
	return *myInstance->myPhysicalDevice;
}

VulkanDevice& VulkanContext::GetDevice()
{
	return *myInstance->myDevice;
}

VulkanSwapChain& VulkanContext::GetSwapChain()
{
	return *myInstance->mySwapChain;
}

vk::PipelineCache& VulkanContext::GetPipelineCache()
{
	return myInstance->myPipelineCache;
}

void VulkanContext::BeginFrame()
{
	myInstance->mySwapChain->BeginFrame();
}

void VulkanContext::EndFrame()
{
	myInstance->mySwapChain->EndFrame();
}

void VulkanContext::CheckValidationLayerSupport()
{
	auto layers = vk::enumerateInstanceLayerProperties();

	for(const auto& layer : myLayers)
	{
		bool foundLayer = false;
		for(const auto& availableLayer : layers)
		{
			if (!strcmp(layer, availableLayer.layerName))
			{
				foundLayer = true;
				break;
			}
		}

		THROW_IF(!foundLayer, "Failed to find required vulkan layers.");
	}
}

void VulkanContext::CheckExtensionSupport()
{
	auto extensionsAvailable = vk::enumerateInstanceExtensionProperties();

	for(const auto& requiredExtension : myExtensions)
	{
		bool foundExtension = false;
		for(const auto& availableExtension : extensionsAvailable)
		{
			if (!strcmp(requiredExtension, availableExtension.extensionName))
				foundExtension = true;
		}

		THROW_IF(!foundExtension, "Failed to find required vulkan extensions.");
	}
}

void VulkanContext::CreateInstance()
{
	std::string appName = String::ToString(Engine::GetEngineProperties().Title);

	vk::ApplicationInfo appInfo = vk::ApplicationInfo()
		.setPApplicationName(appName.c_str())
		.setApplicationVersion(1)
		.setPEngineName("Engine")
		.setEngineVersion(1)
		.setApiVersion(VK_API_VERSION_1_0);

	vk::InstanceCreateInfo instInfo = vk::InstanceCreateInfo()
		.setFlags(vk::InstanceCreateFlags())
		.setPApplicationInfo(&appInfo)
		.setEnabledExtensionCount(static_cast<uint32_t>(myExtensions.size()))
		.setPpEnabledExtensionNames(myExtensions.data())
		.setEnabledLayerCount(static_cast<uint32_t>(myLayers.size()))
		.setPpEnabledLayerNames(myLayers.data());

	myVulkanInstance = vk::createInstance(instInfo);
	LOG("Vulkan instance created successfully.");
}

void VulkanContext::CreateDebugLayer()
{
#ifndef DEBUG
	if(!Engine::GetEngineProperties().HasStartupArgument("-VulkanDebug"))
		return;
#endif

	vk::DebugUtilsMessageSeverityFlagsEXT severityFlags(
		vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo |
		vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
		vk::DebugUtilsMessageSeverityFlagBitsEXT::eError |
		vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose);

	vk::DebugUtilsMessageTypeFlagsEXT messageTypeFlags(
		//vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
		vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
		vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation);

	pfnVkCreateDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(myVulkanInstance.getProcAddr("vkCreateDebugUtilsMessengerEXT"));
	pfnVkDestroyDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(myVulkanInstance.getProcAddr("vkDestroyDebugUtilsMessengerEXT"));

	vk::DebugUtilsMessengerCreateInfoEXT createInfo = vk::DebugUtilsMessengerCreateInfoEXT({}, severityFlags, messageTypeFlags, &DebugMessageCallback, static_cast<void*>(this));
	myDebugMessenger = myVulkanInstance.createDebugUtilsMessengerEXT(createInfo);
	LOG("Vulkan debug layer attached.");
}

VkBool32 VulkanContext::DebugMessageCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{
	std::ostringstream ss;

	ss << vk::to_string(vk::DebugUtilsMessageTypeFlagsEXT(messageType)) << " ";
	ss << pCallbackData->pMessage;
	ss << "\n" << "Object Names: \n";
	for(int i = 0; i < pCallbackData->objectCount; ++i)
	{
		if(pCallbackData->pObjects[i].pObjectName)
			ss << " - " << pCallbackData->pObjects[i].pObjectName << "\n";
	}

	if(vk::DebugUtilsMessageTypeFlagsEXT(messageType) & vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation && !(vk::DebugUtilsMessageSeverityFlagsEXT(messageSeverity) & vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo))
	{
		LOG_ERROR(ss.str());
		check(false);
		return false;
	}

	LOG(ss.str());
	return false;
}