#pragma once

#include "ECS/System.h"
#include "Vulkan/VulkanUniformBuffer.hpp"
#include "ECS/Components/Transform.h"
#include "ECS/Components/Camera.h"

class RenderSystem : public System<const Transform, const Camera>
{
public:
	RenderSystem();
	~RenderSystem();

	virtual void Tick() override final;

	vk::RenderPass& GetRenderPass();

private:
	void CreateRenderPass();
	void CreateFrameBuffer();
	void CreatePipelines();

	void UpdateFrameBuffer();
	void UpdateObjectBuffer(const Transform& inTransform);

	vk::Framebuffer GetFrameBuffer() const;

private:
	class VulkanPipeline* myPipeline = nullptr;

	vk::RenderPass myRenderPass;
	List<vk::Framebuffer> myFrameBuffers;

	class VulkanDepthBuffer* myDepthBuffer = nullptr;

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

	class VulkanImage* myRenderTexture = nullptr;

	class Model* myModel = nullptr;
	class VulkanTexture* myTexture = nullptr;
};