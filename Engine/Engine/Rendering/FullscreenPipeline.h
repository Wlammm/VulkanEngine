#pragma once
#include "Vulkan/VulkanDescriptorSet.h"

class Shader;

class FullscreenPipeline
{
public:
	FullscreenPipeline(Shader* inFragmentShader, class VulkanImage* inSource);
	~FullscreenPipeline();

	void AddFullscreenPass(const vk::CommandBuffer inCommandBuffer);

private:
	void CreatePipeline();
	void CreateDescriptors(class VulkanImage* inSource);
	
	void OnShaderRecompiled();

private:
	Shader* myVertexShader = nullptr;
	Shader* myFragmentShader = nullptr;

	vk::Pipeline myPipeline;
	vk::PipelineLayout myPipelineLayout;

	VulkanDescriptorSet myDescriptorSet;
};