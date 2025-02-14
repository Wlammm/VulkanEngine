#pragma once

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

	vk::CommandPool GetCommandPool() const;
	
	vk::CommandBuffer CreateCommandBuffer(const bool inBegin, const bool inCompute = false);
	void FlushCommandBuffer(vk::CommandBuffer inCommandBuffer);

private:
	List<vk::DeviceQueueCreateInfo> GetQueueFamilyCreateInfos();

	// You cant create 2 queues with the same index. This method is used to make sure we do not add duplicate create infos with same index.
	bool ShouldAddQueueWithIndex(const int inIndex, const List<vk::DeviceQueueCreateInfo>& inCreateInfos) const;

	void CreateCommandPools();

private:
	const VulkanPhysicalDevice& myPhysicalDevice;

	vk::Device myDevice;

	vk::Queue myGraphicsQueue;
	vk::Queue myComputeQueue;
	vk::Queue myTransferQueue;

	vk::CommandPool myCommandPool;
	vk::CommandPool myComputeCommandPool;
};