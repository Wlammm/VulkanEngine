#pragma once

class VulkanShader;

class FullscreenPipeline
{
public:
	FullscreenPipeline(VulkanShader* inFragmentShader);
	~FullscreenPipeline();

private:
	VulkanShader* myVertexShader = nullptr;
	VulkanShader* myFragmentShader = nullptr;

	
};