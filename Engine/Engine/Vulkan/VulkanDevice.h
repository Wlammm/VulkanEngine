#pragma once

#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1
#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.hpp>
#include "Containers/List.hpp"

class VulkanPhysicalDevice;

class VulkanDevice 
{
public:
	VulkanDevice(const VulkanPhysicalDevice& inPhysicalDevice);
	~VulkanDevice();

	vk::Device* operator->() { return &myDevice; }
	const vk::Device* operator->() const { return &myDevice; }

	const vk::Queue& GetGraphicsQueue() const;
	const vk::Queue& GetComputeQueue() const;
	const vk::Queue& GetTransferQueue() const;
	const vk::Queue& GetPresentQueue() const;

private:
	List<vk::DeviceQueueCreateInfo> GetQueueFamilyCreateInfos();

	// You cant create 2 queues with the same index. This method is used to make sure we do not add duplicate create infos with same index.
	bool ShouldAddQueueWithIndex(const int inIndex, const List<vk::DeviceQueueCreateInfo>& inCreateInfos) const;

private:
	const VulkanPhysicalDevice& myPhysicalDevice;

	vk::Device myDevice;

	vk::Queue myGraphicsQueue;
	vk::Queue myComputeQueue;
	vk::Queue myTransferQueue;
};