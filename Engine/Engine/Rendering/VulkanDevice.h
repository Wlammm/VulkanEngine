#pragma once

class VulkanPhysicalDevice;

class VulkanDevice 
{
public:
	VulkanDevice(const VulkanPhysicalDevice& inPhysicalDevice);
	~VulkanDevice();

	vk::Device* operator->() { return &myDevice; }
	const vk::Device* operator->() const { return &myDevice; }

private:
	std::vector<vk::DeviceQueueCreateInfo> GetQueueFamilyCreateInfos();

	// You cant create 2 queues with the same index. This method is used to make sure we do not add duplicate create infos with same index.
	bool ShouldAddQueueWithIndex(const int inIndex, const std::vector<vk::DeviceQueueCreateInfo>& inCreateInfos) const;

private:
	const VulkanPhysicalDevice& myPhysicalDevice;

	vk::Device myDevice;

	vk::Queue myGraphicsQueue;
	vk::Queue myComputeQueue;
	vk::Queue myTransferQueue;
};