#pragma once

#include "Vulkan/VulkanDescriptorSet.h"

class Shader;
class VulkanBuffer;

class DebugPipeline
{
public:
	DebugPipeline();
	~DebugPipeline();

	void AddDrawCommands(const vk::CommandBuffer inCommandBuffer);

private:
	void OnShaderRecompiled();
	
	void CreatePipeline();
	void CreateDescriptorSets();

	VulkanBuffer* BuildVertexBuffer();
	void UpdateFrameBuffer();

private:
	Shader* myVertexShader;
	Shader* myFragmentShader;

	vk::Pipeline myPipeline;
	vk::PipelineLayout myPipelineLayout;

	VulkanDescriptorSet myFrameDescriptorSet;

	struct FrameData
	{
		glm::mat4 myToView;
		glm::mat4 myProjection;
		glm::vec3 myCameraPosition;
	};
	VulkanBuffer* myFrameDataBuffer;

	struct DebugVertex
	{
		glm::vec3 myPosition;
		Color myColor;
	};
};