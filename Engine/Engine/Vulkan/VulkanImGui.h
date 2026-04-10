#pragma once

class VulkanImGui
{
public:
	static void Start(vk::RenderPass inRenderPass);
	static void Destroy();

	static void BeginFrame();
	static void Render(vk::CommandBuffer inCommandBuffer);

	static void ReloadFonts(float inDpiScale);

	inline static ImFont* SourceSansPro_Regular = nullptr;

private:
	static int ImGuiCreateWindowSurfaceImpl(ImGuiViewport* viewport, ImU64 vk_instance, const void* vk_allocator, ImU64* out_vk_surface);
	static void VulkanCheckResult(VkResult result);

	static void LoadFonts(float inDpiScale);

	inline static float myCurrentDpiScale = 1.0f;

	
private:
	inline static VkDescriptorPool myDescriptorPool;

	inline static ImGui_ImplVulkanH_Window myWindow;
};