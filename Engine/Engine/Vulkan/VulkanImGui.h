#pragma once

class VulkanImGui
{
public:
	static void Start();
	static void Destroy();

	static void BeginFrame();
	static void Render(vk::CommandBuffer inCommandBuffer);

private:
	inline static VkDescriptorPool myDescriptorPool;
};