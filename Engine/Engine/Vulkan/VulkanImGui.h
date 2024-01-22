#pragma once

class VulkanImGui
{
public:
	static void Start();
	static void Destroy();

	static void BeginFrame();
	static void Render(vk::CommandBuffer inCommandBuffer);

private:
	static int ImGuiCreateWindowSurfaceImpl(ImGuiViewport* viewport, ImU64 vk_instance, const void* vk_allocator, ImU64* out_vk_surface);
	static void VulkanCheckResult(VkResult result);

private:
	inline static VkDescriptorPool myDescriptorPool;

	inline static ImGui_ImplVulkanH_Window myWindow;
};