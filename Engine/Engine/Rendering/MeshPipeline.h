#pragma once
#include "Vulkan/VulkanUniformBuffer.hpp"

class VulkanShader;

// This pipeline draws all meshes.
class MeshPipeline
{
public:
	MeshPipeline();
	~MeshPipeline();

	void AddDrawCommands(const vk::CommandBuffer inCommandBuffer);

private:
	void CreateDescriptorSetLayouts();
	void CreateDescriptorSets();
	void CreatePipeline();

	void BuildFrameBuffer();

private:
	class VulkanDescriptorSet* myFrameDescriptorSet;

	vk::DescriptorSetLayout myObjectDescriptorSetLayout;
	vk::DescriptorSet myObjectDescriptorSet;

	vk::Pipeline myPipeline;
	vk::PipelineLayout myPipelineLayout;

	VulkanShader* myVertexShader;
	VulkanShader* myFragmentShader;

	struct FrameData
	{
		Mat4f myToView;
		Mat4f myProjection;
	};
	VulkanUniformBuffer<FrameData> myFrameData{vk::ShaderStageFlagBits::eVertex, 0 };

	struct ObjectData
	{
		Mat4f myToWorld;
	};
	VulkanUniformBuffer<ObjectData> myObjectData{ vk::ShaderStageFlagBits::eVertex, 0 };

};