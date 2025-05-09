#include "EnginePch.h"
#include "VulkanDevice.h"

#include "VulkanContext.h"
#include "VulkanPhysicalDevice.h"
#include "Engine/Utils/ThreadUtils.hpp"

VulkanDevice::VulkanDevice(const VulkanPhysicalDevice& inPhysicalDevice)
	: myPhysicalDevice{ inPhysicalDevice }
{
	const List<vk::DeviceQueueCreateInfo> queueCreateInfos = GetQueueFamilyCreateInfos();

	vk::DeviceCreateInfo createInfo = vk::DeviceCreateInfo().setQueueCreateInfos(queueCreateInfos).setPEnabledExtensionNames(myPhysicalDevice.GetExtensions());

	vk::PhysicalDeviceVulkan12Features vulkan12Features{};
	createInfo.pNext = &vulkan12Features;
	vulkan12Features.drawIndirectCount = VK_TRUE;
	vulkan12Features.descriptorBindingPartiallyBound = VK_TRUE;
	vulkan12Features.runtimeDescriptorArray = VK_TRUE;
	vulkan12Features.descriptorBindingSampledImageUpdateAfterBind = VK_TRUE;
	vulkan12Features.descriptorBindingVariableDescriptorCount = VK_TRUE;

	vk::PhysicalDeviceVulkan11Features vulkan11Features{};
	vulkan12Features.pNext = &vulkan11Features;
	vulkan11Features.shaderDrawParameters = VK_TRUE;

	vk::DeviceDiagnosticsConfigCreateInfoNV aftermathInfo = {};
	if(VulkanContext::GetAftermathTracker())
	{
		vulkan11Features.pNext = &aftermathInfo;
		vk::DeviceDiagnosticsConfigFlagsNV aftermathFlags =
			vk::DeviceDiagnosticsConfigFlagBitsNV::eEnableResourceTracking |
			vk::DeviceDiagnosticsConfigFlagBitsNV::eEnableAutomaticCheckpoints |
			vk::DeviceDiagnosticsConfigFlagBitsNV::eEnableShaderDebugInfo;
		aftermathInfo.setFlags(aftermathFlags);
	}
	
	myDevice = myPhysicalDevice->createDevice(createInfo);

	myGraphicsQueue = myDevice.getQueue(myPhysicalDevice.GetGraphicsQueueIndex(), 0);
	myComputeQueue = myDevice.getQueue(myPhysicalDevice.GetComputeQueueIndex(), 0);
	myTransferQueue = myDevice.getQueue(myPhysicalDevice.GetTransferQueueIndex(), 0);

	check(ThreadUtils::IsOnMainThread());
	CreateCommandPoolForThread(std::this_thread::get_id());

	LOG("Logical device created.");
}

VulkanDevice::~VulkanDevice()
{
	std::scoped_lock lock(myCommandPoolsMutex);
	for(auto& [id, commandPool] : myCommandPools)
	{
		myDevice.destroyCommandPool(commandPool);
	}
	myCommandPools.clear();
	
	for(auto& [id, commandPool] : myComputeCommandPools)
	{
		myDevice.destroyCommandPool(commandPool);
	}
	myComputeCommandPools.clear();

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

vk::CommandPool VulkanDevice::GetCommandPoolForThread(const std::thread::id inThreadId) const
{
	std::scoped_lock lock(myCommandPoolsMutex);
	return myCommandPools.at(inThreadId);
}

vk::CommandBuffer VulkanDevice::CreateCommandBufferForThread(const std::thread::id inThreadId, const bool inBegin, const bool isSecondaryBuffer)
{
	std::map<std::thread::id, vk::CommandPool>::iterator iter;
	{
		std::scoped_lock lock(myCommandPoolsMutex);
		iter = myCommandPools.find(inThreadId);
	}

	if(iter == myCommandPools.end())
	{
		CreateCommandPoolForThread(inThreadId);
		iter = myCommandPools.find(inThreadId);
	}

	vk::CommandPool commandPool = iter->second;
	
	vk::CommandBufferAllocateInfo allocInfo = vk::CommandBufferAllocateInfo()
		.setCommandPool(commandPool)
		.setLevel(isSecondaryBuffer ? vk::CommandBufferLevel::eSecondary : vk::CommandBufferLevel::ePrimary)
		.setCommandBufferCount(1);

	vk::CommandBuffer buffer = myDevice.allocateCommandBuffers(allocInfo).front();

	if(inBegin)
	{
		vk::CommandBufferInheritanceInfo inheritanceInfo;
		vk::CommandBufferBeginInfo beginInfo = vk::CommandBufferBeginInfo().setPInheritanceInfo(&inheritanceInfo).setFlags(vk::CommandBufferUsageFlagBits::eSimultaneousUse);
		buffer.begin(beginInfo);
	}

	return buffer;
}

void VulkanDevice::FlushCommandBufferFromThread(const std::thread::id inThreadId, vk::CommandBuffer inCommandBuffer)
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
	
	std::scoped_lock lock(myCommandPoolsMutex);
	myDevice.freeCommandBuffers(myCommandPools.at(inThreadId), inCommandBuffer);
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

void VulkanDevice::CreateCommandPoolForThread(const std::thread::id inThreadId)
{
	std::scoped_lock lock(myCommandPoolsMutex);
	
	vk::CommandPoolCreateInfo createInfo = vk::CommandPoolCreateInfo()
		.setQueueFamilyIndex(myPhysicalDevice.GetGraphicsQueueIndex())
		.setFlags(vk::CommandPoolCreateFlagBits::eResetCommandBuffer);

	check(!myCommandPools.contains(inThreadId));
	myCommandPools.insert({inThreadId, myDevice.createCommandPool(createInfo)});
	
	//createInfo.setQueueFamilyIndex(myPhysicalDevice.GetComputeQueueIndex());
	//
	//check(!myComputeCommandPools.contains(inThreadId));
	//myComputeCommandPools.insert({inThreadId, myDevice.createCommandPool(createInfo)});
}
