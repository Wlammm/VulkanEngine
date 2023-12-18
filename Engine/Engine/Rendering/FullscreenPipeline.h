#pragma once
#include "Assets/AssetObserver.h"
#include "Vulkan/VulkanDescriptorSet.h"

class VulkanShader;
class Transform;

class FullscreenPipeline : public AssetObserver
{
public:
	FullscreenPipeline(VulkanShader* inFragmentShader, class VulkanImage* inSource);
	~FullscreenPipeline();

	void AddFullscreenPass(const vk::CommandBuffer inCommandBuffer);

private:
	void CreatePipeline();
	void CreateDescriptors(class VulkanImage* inSource);
	
	// Inherited via AssetObserver
	virtual void OnAssetUpdated() override final;

private:
	VulkanShader* myVertexShader = nullptr;
	VulkanShader* myFragmentShader = nullptr;

	vk::Pipeline myPipeline;
	vk::PipelineLayout myPipelineLayout;

	vk::RenderPass myRenderPass;

	VulkanDescriptorSet myDescriptorSet;
};