#include "EnginePch.h"
#include "VulkanImGui.h"

#include "VulkanCommandBuffer.h"
#include "Engine/Engine.h"
#include "Engine/Windows/WindowHandler.h"
#include "VulkanContext.h"
#include "VulkanDevice.h"
#include "VulkanPhysicalDevice.h"
#include "VulkanSwapChain.h"
#include "Engine/Rendering/RenderSystem.h"

void VulkanImGui::Start()
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

	ImGui_ImplVulkan_Init(&createInfo, Engine::GetEngineSystem<RenderSystem>().GetImGuiRenderPass());

	VulkanCommandBuffer* cmd = VulkanContext::GetDevice().CreateCommandBuffer(true, false);
	
	LoadFonts();
	
	ImGui_ImplVulkan_CreateFontsTexture(cmd->GetAPIResource());
	VulkanContext::GetDevice().FlushCommandBuffer(cmd);
	ImGui_ImplVulkan_DestroyFontUploadObjects();

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
	ImGui_ImplVulkan_NewFrame();
	ImGui_ImplWin32_NewFrame();

	ImGui::NewFrame();
}

void VulkanImGui::Render(vk::CommandBuffer inCommandBuffer)
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.f);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(43.f / 255.f, 43.f / 255.f, 43.f / 255.f, 100.f / 255.f));
	ImGui::PopStyleVar(1);
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

void VulkanImGui::LoadFonts()
{
	ImGuiIO& io = ImGui::GetIO();
	SourceSansPro_Regular = io.Fonts->AddFontFromFileTTF("Editor/Fonts/SourceSansPro-Regular.ttf", 16.0f);
}
