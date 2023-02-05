#include "EnginePch.h"
#include "VulkanRenderer.h"
#include "Windows/WindowHandler.h"
#include "VulkanContext.h"

VulkanRenderer::VulkanRenderer()
{
	CreateBackBuffer();
}

VulkanRenderer::~VulkanRenderer()
{
	VulkanContext::GetInstance().destroySurfaceKHR(myBackBuffer);
}

void VulkanRenderer::Tick()
{

}

void VulkanRenderer::CreateBackBuffer()
{
	vk::Win32SurfaceCreateInfoKHR createInfo = vk::Win32SurfaceCreateInfoKHR().setHinstance(WindowHandler::GetHInstance()).setHwnd(WindowHandler::GetHWND());
	vk::Result result = VulkanContext::GetInstance().createWin32SurfaceKHR(&createInfo, nullptr, &myBackBuffer);
	check(result == vk::Result::eSuccess);
}
