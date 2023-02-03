#include "EnginePch.h"
#include "VulkanContext.h"
#include "Engine.h"
#include "Utils/String.hpp"

VulkanContext::VulkanContext()
{
	check(!myInstance && "There can only be one vulkan context at the same time.");
	myInstance = this;

	CheckValidationLayerSupport();
	CreateVulkanInstance();
}

VulkanContext::~VulkanContext()
{
	myVulkanInstance.destroy();
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

		if(!foundLayer)
			THROW("Failed to find required vulkan layers.");
	}
}

void VulkanContext::CreateVulkanInstance()
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
