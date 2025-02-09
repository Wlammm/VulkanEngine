#pragma once
#include "Vulkan/VulkanDescriptorSet.h"

class Shader;
class TransformComponent;
class DirectionalLightComponent;

class ShadowPipeline
{
public:
	ShadowPipeline();
	~ShadowPipeline();

	void AddCommands(const vk::CommandBuffer inCommandBuffer);

	vk::RenderPass GetRenderPass() const;

private:
	// Inherited via AssetObserver
	void OnShaderRecompiled();

	void CreateDescriptors();
	void CreateRenderPass();
	void CreatePipeline();

	void BuildFrameBuffer(const TransformComponent* inLightTransform, const DirectionalLightComponent* inLight);

private:
	inline static constexpr vk::Format myShadowMapFormat = vk::Format::eD32Sfloat;

	VulkanDescriptorSet myFrameDescriptorSet;
	
	vk::Pipeline myPipeline;
	vk::PipelineLayout myPipelineLayout;

	vk::RenderPass myRenderPass;

	Shader* myVertexShader;

	struct FrameData
	{
		glm::mat4 myToView;
		glm::mat4 myProjection;
		glm::vec3 myCameraPosition;
	};
	VulkanBuffer* myFrameDataBuffer;

	vk::Framebuffer myDirectionalLightFrameBuffer = nullptr;

	struct PushConstantData
	{
		glm::mat4 myToWorld;
		int myAlbedoIndex;
		int myNormalIndex;
		int myMaterialIndex;
	};
};