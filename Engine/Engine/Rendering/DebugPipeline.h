#pragma once

#include "Assets/AssetObserver.h"
#include "Vulkan/VulkanDescriptorSet.h"

class VulkanShader;
class VulkanBuffer;

class DebugPipeline : public AssetObserver
{
public:
	DebugPipeline();
	~DebugPipeline();

	void AddDrawCommands(const vk::CommandBuffer inCommandBuffer);

private:
	void OnAssetUpdated() override final;
	
	void CreatePipeline();
	void CreateDescriptorSets();

	VulkanBuffer* BuildVertexBuffer();
	void UpdateFrameBuffer();

private:
	VulkanShader* myVertexShader;
	VulkanShader* myFragmentShader;

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