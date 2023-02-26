#pragma once

#define VK_USE_PLATFORM_WIN32_KHR
#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1
#include <vulkan/vulkan.hpp>

#include "Containers/List.hpp"

class VulkanPhysicalDevice 
{
public:
	VulkanPhysicalDevice();
	~VulkanPhysicalDevice();

	const List<const char*>& GetExtensions() const;

	int GetGraphicsQueueIndex() const;
	int GetComputeQueueIndex() const;
	int GetTransferQueueIndex() const;
	int GetPresentQueueIndex() const;

	vk::PhysicalDevice* operator->() { return &myDevice; }
	const vk::PhysicalDevice* operator->() const { return &myDevice; }

	const List<vk::QueueFamilyProperties> GetQueueFamilyProperties() const;

private:
	bool CheckDeviceExtensionSupport(const vk::PhysicalDevice& inDevice) const;

	void GetQueueIndices();

private:
	vk::PhysicalDevice myDevice;
	vk::PhysicalDeviceFeatures myFeatures;
	List<vk::QueueFamilyProperties> myQueueFamilyProperties;

	int myGraphicsQueueIndex = -1;
	int myComputeQueueIndex = -1;
	int myTransferQueueIndex = -1;
	int myPresentQueueIndex = -1;

	const List<const char*> myDeviceExtensions
	{
		VK_KHR_SWAPCHAIN_EXTENSION_NAME,
	};
};