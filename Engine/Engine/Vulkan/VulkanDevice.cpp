#include "EnginePch.h"
#include "VulkanDevice.h"
#include "VulkanPhysicalDevice.h"

VulkanDevice::VulkanDevice(const VulkanPhysicalDevice& inPhysicalDevice)
	: myPhysicalDevice{ inPhysicalDevice }
{
	const List<vk::DeviceQueueCreateInfo> queueCreateInfos = GetQueueFamilyCreateInfos();

	vk::DeviceCreateInfo createInfo = vk::DeviceCreateInfo().setQueueCreateInfos(queueCreateInfos).setPEnabledExtensionNames(myPhysicalDevice.GetExtensions());

	vk::PhysicalDeviceDescriptorIndexingFeatures indexingFeatures{};
	indexingFeatures.descriptorBindingPartiallyBound = VK_TRUE;
	indexingFeatures.runtimeDescriptorArray = VK_TRUE;
	indexingFeatures.descriptorBindingSampledImageUpdateAfterBind = VK_TRUE;
	indexingFeatures.descriptorBindingVariableDescriptorCount = VK_TRUE;

	createInfo.pNext = &indexingFeatures;
	myDevice = myPhysicalDevice->createDevice(createInfo);

	myGraphicsQueue = myDevice.getQueue(myPhysicalDevice.GetGraphicsQueueIndex(), 0);
	myComputeQueue = myDevice.getQueue(myPhysicalDevice.GetComputeQueueIndex(), 0);
	myTransferQueue = myDevice.getQueue(myPhysicalDevice.GetTransferQueueIndex(), 0);

	CreateCommandPools();

	LOG("Logical device created.");
}

VulkanDevice::~VulkanDevice()
{
	myDevice.destroyCommandPool(myCommandPool);
	myDevice.destroyCommandPool(myComputeCommandPool);

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

vk::Device VulkanDevice::GetDevice() const
{
	return myDevice;
}

vk::CommandBuffer VulkanDevice::CreateCommandBuffer(const bool inBegin, const bool inCompute)
{
	vk::CommandBufferAllocateInfo allocInfo = vk::CommandBufferAllocateInfo()
		.setCommandPool(inCompute ? myComputeCommandPool : myCommandPool)
		.setLevel(vk::CommandBufferLevel::ePrimary)
		.setCommandBufferCount(1);

	vk::CommandBuffer buffer = myDevice.allocateCommandBuffers(allocInfo).front();

	if(inBegin)
	{
		vk::CommandBufferBeginInfo beginInfo{};
		buffer.begin(beginInfo);
	}

	return buffer;
}

void VulkanDevice::FlushCommandBuffer(vk::CommandBuffer inCommandBuffer)
{
	const uint64_t DEFAULT_FENCE_TIMEOUT = 100000000000;

	inCommandBuffer.end();

	vk::SubmitInfo submitInfo = vk::SubmitInfo().setCommandBufferCount(1).setCommandBuffers(inCommandBuffer);
	vk::FenceCreateInfo fenceInfo{};
	vk::Fence fence = myDevice.createFence(fenceInfo);

	myGraphicsQueue.submit(submitInfo, fence);
	vk::Result result = myDevice.waitForFences({ fence }, VK_TRUE, DEFAULT_FENCE_TIMEOUT);
	check(result == vk::Result::eSuccess);

	myDevice.destroyFence(fence);
	myDevice.freeCommandBuffers(myCommandPool, inCommandBuffer);
}

List<vk::DeviceQueueCreateInfo> VulkanDevice::GetQueueFamilyCreateInfos()
{
	float priorities = 0.0f;

	List<vk::DeviceQueueCreateInfo> returnVal;

	returnVal.Add(vk::DeviceQueueCreateInfo().setQueueFamilyIndex(myPhysicalDevice.GetGraphicsQueueIndex()).setQueuePriorities(priorities));
	
	if(ShouldAddQueueWithIndex(myPhysicalDevice.GetComputeQueueIndex(), returnVal))
		returnVal.Add(vk::DeviceQueueCreateInfo().setQueueFamilyIndex(myPhysicalDevice.GetComputeQueueIndex()).setQueuePriorities(priorities));
	
	if(ShouldAddQueueWithIndex(myPhysicalDevice.GetTransferQueueIndex(), returnVal))
		returnVal.Add(vk::DeviceQueueCreateInfo().setQueueFamilyIndex(myPhysicalDevice.GetTransferQueueIndex()).setQueuePriorities(priorities));

	return returnVal;
}

bool VulkanDevice::ShouldAddQueueWithIndex(const int inIndex, const List<vk::DeviceQueueCreateInfo>& inCreateInfos) const
{
	for(const auto& entry : inCreateInfos)
	{
		if (entry.queueFamilyIndex == inIndex)
			return false;
	}

	return true;
}

void VulkanDevice::CreateCommandPools()
{
	vk::CommandPoolCreateInfo createInfo = vk::CommandPoolCreateInfo()
		.setQueueFamilyIndex(myPhysicalDevice.GetGraphicsQueueIndex())
		.setFlags(vk::CommandPoolCreateFlagBits::eResetCommandBuffer);

	myCommandPool = myDevice.createCommandPool(createInfo);
	
	createInfo.setQueueFamilyIndex(myPhysicalDevice.GetComputeQueueIndex());
	myComputeCommandPool = myDevice.createCommandPool(createInfo);
}
