#pragma once

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

private:
	vk::DescriptorSetLayout myFrameDescriptorSetLayout;
	vk::DescriptorSetLayout myObjectDescriptorSetLayout;

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
	VulkanUniformBuffer<ObjectData> myObjectData{ vk::ShaderStageFlagBits::eVertex, 1 };

};