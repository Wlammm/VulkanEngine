#pragma once

class VulkanRenderer
{
public:
	VulkanRenderer();
	~VulkanRenderer();

	void Tick();

private:
	vk::SurfaceKHR myBackBuffer;

};