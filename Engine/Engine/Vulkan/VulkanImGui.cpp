#include "EnginePch.h"
#include "VulkanImGui.h"
#include "Engine.h"
#include "Windows/WindowHandler.h"
#include "VulkanContext.h"
#include "VulkanDevice.h"
#include "VulkanPhysicalDevice.h"
#include "VulkanSwapChain.h"

void VulkanImGui::Start()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

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
	createInfo.DescriptorPool = imguiPool;
	createInfo.MinImageCount = 3;
	createInfo.ImageCount = 3;
	createInfo.MSAASamples = VK_SAMPLE_COUNT_1_BIT;

	ImGui_ImplVulkan_Init(&createInfo, VulkanContext::GetSwapChain().GetRenderPass());

	vk::CommandBuffer cmd = VulkanContext::GetDevice().CreateCommandBuffer(true);
	ImGui_ImplVulkan_CreateFontsTexture(cmd);
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
	ImGui::GetIO().DisplaySize = ImGui::GetMainViewport()->WorkSize;

	ImGui::UpdatePlatformWindows();
	ImGui::RenderPlatformWindowsDefault();
	ImGui::EndFrame();
}