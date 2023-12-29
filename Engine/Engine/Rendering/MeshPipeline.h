#pragma once
#include "Vulkan/VulkanUniformBuffer.hpp"
#include "Vulkan/VulkanStorageBuffer.hpp"
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
	void BuildPointLightBuffer();
	void BuildDirectionalLightBuffer();
	void BuildObjectBuffer(const Transform& inTransform);

	virtual void OnAssetUpdated() override final;

private:
	VulkanDescriptorSet myFrameDescriptorSet;
	VulkanDescriptorSet myObjectDescriptorSet;

	vk::Pipeline myPipeline;
	vk::PipelineLayout myPipelineLayout;

	VulkanShader* myVertexShader;
	VulkanShader* myFragmentShader;

	// FrameDescriptorSet.
	struct FrameData
	{
		Mat4f myToView;
		Mat4f myProjection;
		Vec3f myCameraPosition;
	};
	VulkanUniformBuffer<FrameData> myFrameData{vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment, 0 };

	// ObjectDescriptorSet.
	struct ObjectData
	{
		Mat4f myToWorld;
	};
	VulkanUniformBuffer<ObjectData> myObjectData{ vk::ShaderStageFlagBits::eVertex, 0 };

	// FrameDescriptorSet.
	struct alignas(16) PointLightData
	{
		int myNumLights;
		struct alignas(16)
		{
			Color myColor = { 0, 0, 0, 0 };
			Vec3f myPosition = { 0, 0, 0 };
			float myRange = 0;
		} myLights[10];
	} myPointLightData;
	VulkanStorageBuffer<PointLightData> myPointLightDataBuffer{ vk::ShaderStageFlagBits::eFragment, 1 };

	// FrameDescriptorSet.
	struct DirectionalLightBuffer
	{
		Color myColor;
		Vec3f myDirection;
	};
	VulkanUniformBuffer<DirectionalLightBuffer> myDirectionalLightDataBuffer{ vk::ShaderStageFlagBits::eFragment, 2 };
};