#include "EnginePch.h"
#include "VulkanContext.h"
#include "Engine.h"
#include "VulkanPhysicalDevice.h"
#include "VulkanDevice.h"
#include "VulkanSwapchain.h"
#include "Utils/String.hpp"
#include "VulkanAllocator.h"
#include "VulkanImGui.h"
#include "Tracy/tracy/Tracy.hpp"
#include "VulkanUtils.hpp"


PFN_vkCreateDebugUtilsMessengerEXT pfnVkCreateDebugUtilsMessengerEXT;
VKAPI_ATTR VkResult VKAPI_CALL vkCreateDebugUtilsMessengerEXT(VkInstance instance,
	const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
	const VkAllocationCallbacks* pAllocator,
	VkDebugUtilsMessengerEXT* pMessenger)
{
	return pfnVkCreateDebugUtilsMessengerEXT(instance, pCreateInfo, pAllocator, pMessenger);
}

PFN_vkDestroyDebugUtilsMessengerEXT pfnVkDestroyDebugUtilsMessengerEXT;
VKAPI_ATTR void VKAPI_CALL vkDestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT messenger,
	VkAllocationCallbacks const* pAllocator)
{
	return pfnVkDestroyDebugUtilsMessengerEXT(instance, messenger, pAllocator);
}

PFN_vkSetDebugUtilsObjectNameEXT pfnVkSetDebugUtilsObjectNameEXT;
VKAPI_ATTR VkResult VKAPI_CALL vkSetDebugUtilsObjectNameEXT(VkDevice inDevice, VkDebugUtilsObjectNameInfoEXT const* inDebugUtilsObjectNameInfo)
{
	return pfnVkSetDebugUtilsObjectNameEXT(inDevice, inDebugUtilsObjectNameInfo);
}

PFN_vkCmdBeginDebugUtilsLabelEXT pfnVkCmdBeginDebugUtilsLabelEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdBeginDebugUtilsLabelEXT(VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT* pLabelInfo)
{
	return pfnVkCmdBeginDebugUtilsLabelEXT(commandBuffer, pLabelInfo);
}

PFN_vkCmdEndDebugUtilsLabelEXT pfnVkCmdEndDebugUtilsLabelEXT;
VKAPI_ATTR void VKAPI_CALL vkCmdEndDebugUtilsLabelEXT(VkCommandBuffer commandBuffer)
{
	return pfnVkCmdEndDebugUtilsLabelEXT(commandBuffer);
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
	myAllocator = new VulkanAllocator(myVulkanInstance, *myPhysicalDevice, *myDevice);
	mySwapChain = new VulkanSwapChain(*myDevice);

	VulkanUtils::CreateSamplers();
	myPipelineCache = GetDevice()->createPipelineCache(vk::PipelineCacheCreateInfo(), nullptr);
	CreateDescriptorPool();
}

VulkanContext::~VulkanContext()
{
	VulkanImGui::Destroy();

	// This will destroy all descriptor sets and layouts that are allocated from it.
	GetDevice()->destroyDescriptorPool(myDescriptorPool);
	GetDevice()->destroyPipelineCache(myPipelineCache);
	VulkanUtils::DestroySamplers();

	del(mySwapChain);
	del(myAllocator);
	del(myDevice);
	del(myPhysicalDevice);
	DestroyDebugLayer();
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

VulkanAllocator& VulkanContext::GetAllocator()
{
	return *myInstance->myAllocator;
}

glm::vec2 VulkanContext::GetRenderResolution()
{
	return glm::vec2{ static_cast<float>(GetSwapChain().GetWidth()), static_cast<float>(GetSwapChain().GetHeight()) };
}

void VulkanContext::BeginFrame()
{
	ZoneScoped;
	myInstance->mySwapChain->BeginFrame();
	VulkanImGui::BeginFrame();
	myInstance->myAllocator->Tick();
}

void VulkanContext::EndFrame()
{
	myInstance->mySwapChain->EndFrame();
}

vk::DescriptorPool VulkanContext::GetDescriptorPool()
{
	return myInstance->myDescriptorPool;
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
		.setApiVersion(USED_VULKAN_VERSION);
	
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
	pfnVkSetDebugUtilsObjectNameEXT = reinterpret_cast<PFN_vkSetDebugUtilsObjectNameEXT>(myVulkanInstance.getProcAddr("vkSetDebugUtilsObjectNameEXT"));
	pfnVkCmdBeginDebugUtilsLabelEXT = reinterpret_cast<PFN_vkCmdBeginDebugUtilsLabelEXT>(myVulkanInstance.getProcAddr("vkCmdBeginDebugUtilsLabelEXT"));
	pfnVkCmdEndDebugUtilsLabelEXT = reinterpret_cast<PFN_vkCmdEndDebugUtilsLabelEXT>(myVulkanInstance.getProcAddr("vkCmdEndDebugUtilsLabelEXT"));

	vk::DebugUtilsMessengerCreateInfoEXT createInfo = vk::DebugUtilsMessengerCreateInfoEXT({}, severityFlags, messageTypeFlags, &DebugMessageCallback, static_cast<void*>(this));
	myDebugMessenger = myVulkanInstance.createDebugUtilsMessengerEXT(createInfo);
	LOG("Vulkan debug layer attached.");
}

void VulkanContext::DestroyDebugLayer()
{
#ifndef DEBUG
	if (!Engine::GetEngineProperties().HasStartupArgument("-VulkanDebug"))
		return;
#endif
	myVulkanInstance.destroyDebugUtilsMessengerEXT(myDebugMessenger);
}

void VulkanContext::CreateDescriptorPool()
{
	List<vk::DescriptorPoolSize> poolSizes;
	poolSizes.Emplace().setDescriptorCount(100).setType(vk::DescriptorType::eUniformBuffer);
	poolSizes.Emplace().setDescriptorCount(100).setType(vk::DescriptorType::eCombinedImageSampler);
	vk::DescriptorPoolCreateInfo createInfo = vk::DescriptorPoolCreateInfo().setPoolSizes(poolSizes).setMaxSets(1000);

	myDescriptorPool = GetDevice()->createDescriptorPool(createInfo);
}

VkBool32 VulkanContext::DebugMessageCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{
	std::ostringstream ss;

	ss << vk::to_string(vk::DebugUtilsMessageTypeFlagsEXT(messageType)) << " ";
	ss << pCallbackData->pMessage;
	ss << "\n" << "Object Names: \n";
	for(uint i = 0; i < pCallbackData->objectCount; ++i)
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