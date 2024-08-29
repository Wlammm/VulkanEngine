#pragma once
#include "Vulkan/VulkanDescriptorSet.h"

class TransformComponent;
class Shader;
class Transform;

// This pipeline draws all meshes.
class MeshPipeline
{
public:
	MeshPipeline();
	~MeshPipeline();

	void AddDrawCommands(const vk::CommandBuffer inCommandBuffer);

private:
	void CreateDescriptors();
	void CreatePipeline();

	void BuildFrameBuffer();
	void BuildDirectionalLightBuffer();

	void OnShaderRecompiled();

private:
	VulkanDescriptorSet myFrameDescriptorSet;

	vk::Pipeline myPipeline;
	vk::PipelineLayout myPipelineLayout;

	Shader* myVertexShader;
	Shader* myFragmentShader;

	// FrameDescriptorSet.
	struct FrameData
	{
		glm::mat4 myToView;
		glm::mat4 myProjection;
		glm::vec3 myCameraPosition;
	};
	VulkanBuffer* myFrameDataBuffer; 

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