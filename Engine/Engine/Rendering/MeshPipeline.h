#pragma once
#include "Assets/AssetObserver.h"
#include "Vulkan/VulkanDescriptorSet.h"

class TransformComponent;
class VulkanShader;
class Transform;

// This pipeline draws all meshes.
class MeshPipeline : public AssetObserver
{
public:
	MeshPipeline();
	~MeshPipeline();

	void AddDrawCommands(const vk::CommandBuffer inCommandBuffer);

private:
	void CreateDescriptors();
	void CreatePipeline();

	void BuildFrameBuffer();
	void BuildPointLightBuffer();
	void BuildDirectionalLightBuffer();

	virtual void OnAssetUpdated() override final;

private:
	VulkanDescriptorSet myFrameDescriptorSet;

	vk::Pipeline myPipeline;
	vk::PipelineLayout myPipelineLayout;

	VulkanShader* myVertexShader;
	VulkanShader* myFragmentShader;

	// FrameDescriptorSet.
	struct FrameData
	{
		glm::mat4 myToView;
		glm::mat4 myProjection;
		glm::vec3 myCameraPosition;
	};
	VulkanBuffer* myFrameDataBuffer; 

	// FrameDescriptorSet.
	struct alignas(16) PointLightData
	{
		int myNumLights;
		struct alignas(16)
		{
			glm::vec4 myColor = { 0, 0, 0, 0 };
			glm::vec3 myPosition = { 0, 0, 0 };
			float myRange = 0;
		} myLights[10];
	} myPointLightData;
	VulkanBuffer* myPointLightBuffer;

	// FrameDescriptorSet.
	struct DirectionalLightBuffer
	{
		glm::vec4 myColor;
		glm::vec3 myDirection;
		float padding;
		glm::mat4 myLightView;
		glm::mat4 myLightProjection;
	};
	VulkanBuffer* myDirectionalLightBuffer;

	struct PushConstantData
	{
		glm::mat4 myToWorld;
		int myAlbedoIndex;
		int myNormalIndex;
		int myMaterialIndex;
	};
};