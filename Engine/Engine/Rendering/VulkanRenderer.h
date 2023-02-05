#pragma once

class VulkanRenderer
{
public:
	VulkanRenderer();
	~VulkanRenderer();

	void Tick();

private:
	void CreateBackBuffer();

private:
	vk::SurfaceKHR myBackBuffer;

};