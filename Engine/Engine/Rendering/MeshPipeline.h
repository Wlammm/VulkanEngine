#pragma once
#include "Vulkan/VulkanUniformBuffer.hpp"
#include "Vulkan/VulkanDescriptorSet.h"
#include "Assets/AssetObserver.h"

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
	void BuildObjectBuffer(const Transform& inTransform);

	virtual void OnAssetUpdated() override final;

private:
	VulkanDescriptorSet myFrameDescriptorSet;
	VulkanDescriptorSet myObjectDescriptorSet;

	vk::Pipeline myPipeline;
	vk::PipelineLayout myPipelineLayout;

	VulkanShader* myVertexShader;
	VulkanShader* myFragmentShader;

	struct FrameData
	{
		Mat4f myToView;
		Mat4f myProjection;
		Vec3f myCameraPosition;
	};
	VulkanUniformBuffer<FrameData> myFrameData{vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment, 0 };

	struct ObjectData
	{
		Mat4f myToWorld;
	};
	VulkanUniformBuffer<ObjectData> myObjectData{ vk::ShaderStageFlagBits::eVertex, 0 };
};