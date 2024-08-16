#pragma once
#include "Assets/AssetObserver.h"
#include "Vulkan/VulkanDescriptorSet.h"

class VulkanShader;
class TransformComponent;
class DirectionalLightComponent;

class ShadowPipeline : public AssetObserver
{
public:
	ShadowPipeline();
	~ShadowPipeline();

	void AddCommands(const vk::CommandBuffer inCommandBuffer);

	vk::RenderPass GetRenderPass() const;

private:
	// Inherited via AssetObserver
	virtual void OnAssetUpdated() override;

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

	VulkanShader* myVertexShader;

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