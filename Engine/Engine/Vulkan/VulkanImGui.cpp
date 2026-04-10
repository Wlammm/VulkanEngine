#include "EnginePch.h"
#include "VulkanImGui.h"

#include "ImGuiNotify.hpp"
#include "IconsFontAwesome6.h"
#include "fa_solid_900.h"

#include "VulkanCommandBuffer.h"
#include "Engine/Engine.h"
#include "Engine/Windows/WindowHandler.h"
#include "VulkanContext.h"
#include "VulkanDevice.h"
#include "VulkanPhysicalDevice.h"
#include "VulkanSwapChain.h"
#include "Engine/Rendering/RenderSystem.h"

void VulkanImGui::Start(vk::RenderPass inRenderPass)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	//ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	//ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	ImGui::GetPlatformIO().Platform_CreateVkSurface = ImGuiCreateWindowSurfaceImpl;

	ImGui_ImplWin32_Init(Engine::GetWindowHandler().GetHWND());

	VkDescriptorPoolSize pool_sizes[] =
	{
		{ VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
		{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
		{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
		{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
	};

	VkDescriptorPoolCreateInfo pool_info = {};
	pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
	pool_info.maxSets = 1000;
	pool_info.poolSizeCount = static_cast<uint>(std::size(pool_sizes));
	pool_info.pPoolSizes = pool_sizes;

	VkDescriptorPool imguiPool;
	vkCreateDescriptorPool(VulkanContext::GetDevice().GetDevice(), &pool_info, nullptr, &imguiPool);

	ImGui_ImplVulkan_InitInfo createInfo{};
	createInfo.Instance = VulkanContext::GetInstance();
	createInfo.PhysicalDevice = VulkanContext::GetPhysicalDevice().GetPhysicalDevice();
	createInfo.Device = VulkanContext::GetDevice().GetDevice();
	createInfo.Queue = VulkanContext::GetDevice().GetGraphicsQueue();
	createInfo.QueueFamily = VulkanContext::GetPhysicalDevice().GetGraphicsQueueIndex();
	createInfo.DescriptorPool = imguiPool;
	createInfo.MinImageCount = VulkanContext::GetSwapChain().GetMinImageCount();
	createInfo.ImageCount = VulkanContext::FrameLag;
	createInfo.MSAASamples = static_cast<VkSampleCountFlagBits>(vk::SampleCountFlagBits::e1);
	createInfo.CheckVkResultFn = VulkanCheckResult;

	ImGui_ImplVulkan_Init(&createInfo, inRenderPass);

	HWND hwnd = Engine::GetWindowHandler().GetHWND();
	float dpiScale = GetDpiForWindow(hwnd) / 96.0f;

	VulkanCommandBuffer* cmd = VulkanContext::GetDevice().CreateCommandBuffer(true, false);

	LoadFonts(dpiScale);

	ImGui_ImplVulkan_CreateFontsTexture(cmd->GetAPIResource());
	VulkanContext::GetDevice().FlushCommandBuffer(cmd);
	ImGui_ImplVulkan_DestroyFontUploadObjects();

	ImGui::GetStyle().ScaleAllSizes(dpiScale);
	ImGui::GetStyle().WindowMenuButtonPosition = ImGuiDir_None;
	
	myCurrentDpiScale = dpiScale;

	myDescriptorPool = imguiPool;
}

void VulkanImGui::Destroy()
{
	vkDestroyDescriptorPool(VulkanContext::GetDevice().GetDevice(), myDescriptorPool, nullptr);
	ImGui_ImplVulkan_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void VulkanImGui::BeginFrame()
{
	float newDpiScale;
	if (WindowHandler::ConsumeDpiChange(newDpiScale))
		ReloadFonts(newDpiScale);

	ImGui_ImplVulkan_NewFrame();
	ImGui_ImplWin32_NewFrame();

	ImGui::NewFrame();
}

void VulkanImGui::Render(vk::CommandBuffer inCommandBuffer)
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f); // Disable round borders
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f); // Disable borders
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.10f, 0.10f, 0.10f, 1.00f)); // Background color
	ImGui::RenderNotifications();
	ImGui::PopStyleVar(2);
	ImGui::PopStyleColor(1);
	
	ImGui::Render();
	ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), inCommandBuffer);
}

int VulkanImGui::ImGuiCreateWindowSurfaceImpl(ImGuiViewport* viewport, ImU64 vk_instance, const void* vk_allocator, ImU64* out_vk_surface)
{
	vk::SurfaceKHR surface;
	HWND wnd = (HWND)viewport->PlatformHandleRaw;
	HINSTANCE inst = (HINSTANCE)GetWindowLongPtr(wnd, GWLP_HINSTANCE);
	VkWin32SurfaceCreateInfoKHR createInfo = vk::Win32SurfaceCreateInfoKHR().setHinstance(inst).setHwnd(wnd);
	VkResult result = vkCreateWin32SurfaceKHR((VkInstance)vk_instance, &createInfo, (VkAllocationCallbacks*)vk_allocator, (VkSurfaceKHR*)out_vk_surface);
	return (int)result;
}

void VulkanImGui::VulkanCheckResult(VkResult result)
{
	check(result == VkResult::VK_SUCCESS);
}

void VulkanImGui::ReloadFonts(float inDpiScale)
{
	vkDeviceWaitIdle(VulkanContext::GetDevice().GetDevice());

	ImGui_ImplVulkan_DestroyFontTexture();
	ImGui::GetIO().Fonts->Clear();

	VulkanCommandBuffer* cmd = VulkanContext::GetDevice().CreateCommandBuffer(true, false);
	LoadFonts(inDpiScale);
	ImGui_ImplVulkan_CreateFontsTexture(cmd->GetAPIResource());
	VulkanContext::GetDevice().FlushCommandBuffer(cmd);
	ImGui_ImplVulkan_DestroyFontUploadObjects();

	ImGui::GetStyle().ScaleAllSizes(inDpiScale / myCurrentDpiScale);
	myCurrentDpiScale = inDpiScale;
}

void VulkanImGui::LoadFonts(float inDpiScale)
{
	ImGuiIO& io = ImGui::GetIO();

	float baseFontSize = 16.0f * inDpiScale;

	SourceSansPro_Regular = io.Fonts->AddFontFromFileTTF("Editor/Fonts/SourceSansPro-Regular.ttf", baseFontSize);
	io.Fonts->AddFontDefault();

	// ImGuiNotify init...
	float iconFontSize = baseFontSize * 2.0f / 3.0f; // FontAwesome fonts need to have their sizes reduced by 2.0f/3.0f in order to align correctly

	// Check if FONT_ICON_FILE_NAME_FAS is a valid path
	std::ifstream fontAwesomeFile(FONT_ICON_FILE_NAME_FAS);

	if (!fontAwesomeFile.good())
	{
		// If it's not good, then we can't find the font and should abort
		std::cerr << "Could not find the FontAwesome font file." << std::endl;
		abort();
	}

	static const ImWchar iconsRanges[] = {ICON_MIN_FA, ICON_MAX_16_FA, 0};
	ImFontConfig iconsConfig;
	iconsConfig.MergeMode = true;
	iconsConfig.PixelSnapH = true;
	iconsConfig.GlyphMinAdvanceX = iconFontSize;
	io.Fonts->AddFontFromFileTTF(FONT_ICON_FILE_NAME_FAS, iconFontSize, &iconsConfig, iconsRanges);
}
