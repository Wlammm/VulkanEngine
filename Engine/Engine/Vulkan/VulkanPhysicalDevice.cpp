#include "EnginePch.h"
#include "VulkanPhysicalDevice.h"
#include "VulkanContext.h"

VulkanPhysicalDevice::VulkanPhysicalDevice()
{
	vk::Instance& instance = VulkanContext::GetInstance();

	List<vk::PhysicalDevice> physicalDevices = instance.enumeratePhysicalDevices();
	THROW_IF(physicalDevices.IsEmpty(), "No physical devices available.");

	int pickedDeviceIndex = -1;
	for (int i = 0; i < physicalDevices.size(); ++i)
	{
		const vk::PhysicalDevice& device = physicalDevices[i];
		if (device.getProperties().deviceType == vk::PhysicalDeviceType::eDiscreteGpu && CheckDeviceExtensionSupport(device))
		{
			pickedDeviceIndex = i;
			break;
		}
	}
	THROW_IF(pickedDeviceIndex == -1, "Failed to find suitable physical device.");
	myDevice = physicalDevices[pickedDeviceIndex];

	myFeatures = myDevice.getFeatures();
	myQueueFamilyProperties = myDevice.getQueueFamilyProperties();
	myMemoryProperties = myDevice.getMemoryProperties();
	GetQueueIndices();
}

VulkanPhysicalDevice::~VulkanPhysicalDevice()
{
}

const List<const char*>& VulkanPhysicalDevice::GetExtensions() const
{
	return myDeviceExtensions;
}

int VulkanPhysicalDevice::GetGraphicsQueueIndex() const
{
	return myGraphicsQueueIndex;
}

int VulkanPhysicalDevice::GetComputeQueueIndex() const
{
	return myComputeQueueIndex;
}

int VulkanPhysicalDevice::GetTransferQueueIndex() const
{
	return myTransferQueueIndex;
}

int VulkanPhysicalDevice::GetPresentQueueIndex() const
{
	return myPresentQueueIndex;
}

const List<vk::QueueFamilyProperties> VulkanPhysicalDevice::GetQueueFamilyProperties() const
{
	return myQueueFamilyProperties;
}

const vk::PhysicalDeviceMemoryProperties& VulkanPhysicalDevice::GetMemoryProperties() const
{
	return myMemoryProperties;
}

bool VulkanPhysicalDevice::CheckDeviceExtensionSupport(const vk::PhysicalDevice& inDevice) const
{
	const auto availableExtensions = inDevice.enumerateDeviceExtensionProperties();

	for (const auto& requiredExtension : myDeviceExtensions)
	{
		bool found = false;
		for (const auto& extension : availableExtensions)
		{
			if (!strcmp(requiredExtension, extension.extensionName))
				found = true;
		}

		if (!found)
			return false;
	}

	return true;
}

void VulkanPhysicalDevice::GetQueueIndices()
{
	// Check for queue with only graphics support.
	for (int i = 0; i < myQueueFamilyProperties.size(); ++i)
	{
		const vk::QueueFamilyProperties& queueFamily = myQueueFamilyProperties[i];
		if (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics && !(queueFamily.queueFlags & vk::QueueFlagBits::eCompute || queueFamily.queueFlags & vk::QueueFlagBits::eTransfer))
		{
			myGraphicsQueueIndex = i;
			break;
		}
	}

	if(myGraphicsQueueIndex == -1)
	{
		// Pick first queue with graphics support.
		for (int i = 0; i < myQueueFamilyProperties.size(); ++i)
		{
			const vk::QueueFamilyProperties& queueFamily = myQueueFamilyProperties[i];
			if (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics)
			{
				myGraphicsQueueIndex = i;
				break;
			}
		}
	}
	THROW_IF(myGraphicsQueueIndex == -1, "Could not find a graphics queue.");
	
	// Check for queue with only compute support.
	for (int i = 0; i < myQueueFamilyProperties.size(); ++i)
	{
		const vk::QueueFamilyProperties& queueFamily = myQueueFamilyProperties[i];
		if (queueFamily.queueFlags & vk::QueueFlagBits::eCompute && !(queueFamily.queueFlags & vk::QueueFlagBits::eGraphics || queueFamily.queueFlags & vk::QueueFlagBits::eTransfer))
		{
			myComputeQueueIndex = i;
			break;
		}
	}

	if (myComputeQueueIndex == -1)
	{
		// Pick first queue with compute support.
		for (int i = 0; i < myQueueFamilyProperties.size(); ++i)
		{
			const vk::QueueFamilyProperties& queueFamily = myQueueFamilyProperties[i];
			if (queueFamily.queueFlags & vk::QueueFlagBits::eCompute)
			{
				myComputeQueueIndex = i;
				break;
			}
		}
	}
	THROW_IF(myComputeQueueIndex == -1, "Could not find a compute queue.");


	// Check for queue with only transfer support.
	for (int i = 0; i < myQueueFamilyProperties.size(); ++i)
	{
		const vk::QueueFamilyProperties& queueFamily = myQueueFamilyProperties[i];
		if (queueFamily.queueFlags & vk::QueueFlagBits::eTransfer && !(queueFamily.queueFlags & vk::QueueFlagBits::eGraphics || queueFamily.queueFlags & vk::QueueFlagBits::eCompute))
		{
			myTransferQueueIndex = i;
			break;
		}
	}

	if (myTransferQueueIndex == -1)
	{
		// Pick first queue with transfer support.
		for (int i = 0; i < myQueueFamilyProperties.size(); ++i)
		{
			const vk::QueueFamilyProperties& queueFamily = myQueueFamilyProperties[i];
			if (queueFamily.queueFlags & vk::QueueFlagBits::eTransfer)
			{
				myTransferQueueIndex = i;
				break;
			}
		}
	}
	THROW_IF(myTransferQueueIndex == -1, "Could not find a transfer queue.");
}