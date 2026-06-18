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

	// Effective UI scale currently applied to fonts + style (DPI * user zoom).
	static float GetCurrentDpiScale() { return myAppliedScale; }

	// User-controllable zoom on top of the monitor DPI (1.0 == 100%). The change is
	// applied at the start of the next frame so it never rebuilds the font atlas mid-frame.
	static void RequestUserScale(float inUserScale);
	static float GetUserScale() { return myUserScale; }

private:
	static int ImGuiCreateWindowSurfaceImpl(ImGuiViewport* viewport, ImU64 vk_instance, const void* vk_allocator, ImU64* out_vk_surface);
	static void VulkanCheckResult(VkResult result);

	static void LoadFonts(float inDpiScale);
	// Rebuilds the font atlas and rescales the style to a new effective scale.
	static void ApplyScale(float inEffectiveScale);

	inline static float myDpiScale = 1.0f;      // monitor DPI scale (96 dpi == 1.0)
	inline static float myUserScale = 1.0f;     // user zoom multiplier
	inline static float myAppliedScale = 1.0f;  // last effective scale applied to fonts/style
	inline static bool myUserScaleDirty = false;

	
private:
	inline static VkDescriptorPool myDescriptorPool;

	inline static ImGui_ImplVulkanH_Window myWindow;
};