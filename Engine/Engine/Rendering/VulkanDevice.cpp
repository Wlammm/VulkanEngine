#include "EnginePch.h"
#include "VulkanDevice.h"
#include "VulkanPhysicalDevice.h"

VulkanDevice::VulkanDevice(const VulkanPhysicalDevice& inPhysicalDevice)
	: myPhysicalDevice{ inPhysicalDevice }
{
	const std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos = GetQueueFamilyCreateInfos();

	vk::DeviceCreateInfo createInfo = vk::DeviceCreateInfo().setQueueCreateInfos(queueCreateInfos).setPEnabledExtensionNames(myPhysicalDevice.GetExtensions());
	myDevice = myPhysicalDevice->createDevice(createInfo);

	myGraphicsQueue = myDevice.getQueue(myPhysicalDevice.GetGraphicsQueueIndex(), 0);
	myComputeQueue = myDevice.getQueue(myPhysicalDevice.GetComputeQueueIndex(), 0);
	myTransferQueue = myDevice.getQueue(myPhysicalDevice.GetTransferQueueIndex(), 0);

	LOG("Logical device created.");
}

VulkanDevice::~VulkanDevice()
{
	myDevice.destroy();
}

const vk::Queue& VulkanDevice::GetGraphicsQueue() const
{
	return myGraphicsQueue;
}

const vk::Queue& VulkanDevice::GetComputeQueue() const
{
	return myComputeQueue;
}

const vk::Queue& VulkanDevice::GetTransferQueue() const
{
	return myTransferQueue;
}

const vk::Queue& VulkanDevice::GetPresentQueue() const
{
	// Return graphics queue temporarily until we support separate present queues.
	return myGraphicsQueue;
}

std::vector<vk::DeviceQueueCreateInfo> VulkanDevice::GetQueueFamilyCreateInfos()
{
	float priorities = 0.0f;

	std::vector<vk::DeviceQueueCreateInfo> returnVal;

	returnVal.push_back(vk::DeviceQueueCreateInfo().setQueueFamilyIndex(myPhysicalDevice.GetGraphicsQueueIndex()).setQueuePriorities(priorities));
	
	if(ShouldAddQueueWithIndex(myPhysicalDevice.GetComputeQueueIndex(), returnVal))
		returnVal.push_back(vk::DeviceQueueCreateInfo().setQueueFamilyIndex(myPhysicalDevice.GetComputeQueueIndex()).setQueuePriorities(priorities));
	
	if(ShouldAddQueueWithIndex(myPhysicalDevice.GetTransferQueueIndex(), returnVal))
		returnVal.push_back(vk::DeviceQueueCreateInfo().setQueueFamilyIndex(myPhysicalDevice.GetTransferQueueIndex()).setQueuePriorities(priorities));

	return returnVal;
}

bool VulkanDevice::ShouldAddQueueWithIndex(const int inIndex, const std::vector<vk::DeviceQueueCreateInfo>& inCreateInfos) const
{
	for(const auto& entry : inCreateInfos)
	{
		if (entry.queueFamilyIndex == inIndex)
			return false;
	}

	return true;
}
