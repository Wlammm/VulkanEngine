#include "EnginePch.h"
#include "VulkanDevice.h"

#include "VulkanAllocator.h"
#include "VulkanCommandBuffer.h"
#include "VulkanContext.h"
#include "VulkanPhysicalDevice.h"
#include "Engine/Utils/ThreadUtils.hpp"

VulkanDevice::VulkanDevice(const VulkanPhysicalDevice& inPhysicalDevice)
	: myPhysicalDevice{ inPhysicalDevice }
{
	const List<vk::DeviceQueueCreateInfo> queueCreateInfos = GetQueueFamilyCreateInfos();

	vk::DeviceCreateInfo createInfo = vk::DeviceCreateInfo().setQueueCreateInfos(queueCreateInfos).setPEnabledExtensionNames(myPhysicalDevice.GetExtensions());
	
	vk::PhysicalDeviceFeatures physicalDeviceFeatures{};
	physicalDeviceFeatures.samplerAnisotropy = VK_TRUE;
	
	createInfo.pEnabledFeatures = &physicalDeviceFeatures;
	
	vk::PhysicalDeviceVulkan12Features vulkan12Features{};
	createInfo.pNext = &vulkan12Features;
	vulkan12Features.drawIndirectCount = VK_TRUE;
	vulkan12Features.descriptorBindingPartiallyBound = VK_TRUE;
	vulkan12Features.runtimeDescriptorArray = VK_TRUE;
	vulkan12Features.descriptorBindingVariableDescriptorCount = VK_TRUE;
	
	vulkan12Features.scalarBlockLayout = VK_TRUE;
	vulkan12Features.descriptorBindingStorageBufferUpdateAfterBind = VK_TRUE;
	vulkan12Features.descriptorBindingSampledImageUpdateAfterBind = VK_TRUE;
	vulkan12Features.descriptorBindingUniformBufferUpdateAfterBind = VK_TRUE;
	vulkan12Features.descriptorBindingStorageTexelBufferUpdateAfterBind = VK_TRUE;
	vulkan12Features.descriptorBindingUniformTexelBufferUpdateAfterBind = VK_TRUE;
	
	vk::PhysicalDeviceVulkan13Features vulkan13Features{};
	vulkan12Features.pNext = &vulkan13Features;
	vulkan13Features.shaderDemoteToHelperInvocation = VK_TRUE;
	vulkan13Features.dynamicRendering = VK_TRUE;
	
	vk::PhysicalDeviceVulkan11Features vulkan11Features{};
	vulkan13Features.pNext = &vulkan11Features;
	vulkan11Features.shaderDrawParameters = VK_TRUE;
	
	
	vk::DeviceDiagnosticsConfigCreateInfoNV aftermathInfo = {};
	if(VulkanContext::GetAftermathTracker())
	{
		vulkan11Features.pNext = &aftermathInfo;
		vk::DeviceDiagnosticsConfigFlagsNV aftermathFlags =
			vk::DeviceDiagnosticsConfigFlagBitsNV::eEnableResourceTracking |
			vk::DeviceDiagnosticsConfigFlagBitsNV::eEnableAutomaticCheckpoints |
			vk::DeviceDiagnosticsConfigFlagBitsNV::eEnableShaderDebugInfo |
				vk::DeviceDiagnosticsConfigFlagBitsNV::eEnableShaderErrorReporting;
		aftermathInfo.setFlags(aftermathFlags);
	}
	
	
	myDevice = myPhysicalDevice->createDevice(createInfo);

	myGraphicsQueue = myDevice.getQueue(myPhysicalDevice.GetGraphicsQueueIndex(), 0);
	myComputeQueue = myDevice.getQueue(myPhysicalDevice.GetComputeQueueIndex(), 0);
	myTransferQueue = myDevice.getQueue(myPhysicalDevice.GetTransferQueueIndex(), 0);

	check(ThreadUtils::IsOnMainThread());
	
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

vk::Device VulkanDevice::GetDevice() const
{
	return myDevice;
}

VulkanCommandBuffer* VulkanDevice::CreateCommandBuffer(const bool inBegin, const bool isSecondaryBuffer)
{
	vk::CommandPool commandPool = CreateCommandPool();
	
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

	return new VulkanCommandBuffer(buffer, commandPool);
}

void VulkanDevice::FlushCommandBuffer(VulkanCommandBuffer* inCommandBuffer)
{
	const uint64_t DEFAULT_FENCE_TIMEOUT = 100000000000;

	inCommandBuffer->GetAPIResource().end();

	vk::CommandBuffer buffer = inCommandBuffer->GetAPIResource();
	
	vk::SubmitInfo submitInfo = vk::SubmitInfo().setCommandBufferCount(1).setCommandBuffers(buffer);
	vk::FenceCreateInfo fenceInfo{};
	vk::Fence fence = myDevice.createFence(fenceInfo);

	myGraphicsQueue.submit(submitInfo, fence);
	vk::Result result = myDevice.waitForFences({ fence }, VK_TRUE, DEFAULT_FENCE_TIMEOUT);
	check(result == vk::Result::eSuccess);

	myDevice.destroyFence(fence);
	del(inCommandBuffer);
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

vk::CommandPool VulkanDevice::CreateCommandPool()
{
	vk::CommandPoolCreateInfo createInfo = vk::CommandPoolCreateInfo()
		.setQueueFamilyIndex(myPhysicalDevice.GetGraphicsQueueIndex())
		.setFlags(vk::CommandPoolCreateFlagBits::eResetCommandBuffer);
	return myDevice.createCommandPool(createInfo);
}
