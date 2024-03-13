#pragma once
#include "Assets/AssetObserver.h"
#include "Vulkan/VulkanDescriptorSet.h"

class VulkanShader;
class Transform;
struct DirectionalLight;

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

	void BuildFrameBuffer(const Transform& inLightTransform, const DirectionalLight& inLight);
	void BuildObjectBuffer(const Transform& inTransform);

private:
	inline static constexpr vk::Format myShadowMapFormat = vk::Format::eD32Sfloat;

	VulkanDescriptorSet myFrameDescriptorSet;
	VulkanDescriptorSet myObjectDescriptorSet;

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

	struct ObjectData
	{
		glm::mat4 myToWorld;
	};
	VulkanBuffer* myObjectDataBuffer;
};