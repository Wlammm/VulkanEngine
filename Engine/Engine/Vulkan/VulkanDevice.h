#pragma once

class VulkanCommandBuffer;
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
	
	vk::Device GetDevice() const;

	VulkanCommandBuffer* CreateCommandBuffer(const bool inBegin, const bool isSecondaryBuffer = false);
	void FlushCommandBuffer(VulkanCommandBuffer* inCommandBuffer);
	void FlushSecondaryCommandBuffers(const List<VulkanCommandBuffer*>& inCommandBuffers);

private:
	List<vk::DeviceQueueCreateInfo> GetQueueFamilyCreateInfos();

	// You cant create 2 queues with the same index. This method is used to make sure we do not add duplicate create infos with same index.
	bool ShouldAddQueueWithIndex(const int inIndex, const List<vk::DeviceQueueCreateInfo>& inCreateInfos) const;

	vk::CommandPool CreateCommandPool();

private:
	const VulkanPhysicalDevice& myPhysicalDevice;

	vk::Device myDevice;

	vk::Queue myGraphicsQueue;
	vk::Queue myComputeQueue;
	vk::Queue myTransferQueue;
};