#pragma once

#include "Engine/ECS/System.h"
#include "Engine/Vulkan/VulkanUniformBuffer.hpp"
#include "Engine/ECS/Components/Transform.h"
#include "Engine/ECS/Components/Camera.h"
#include "Engine/Events/EventObserver.h"

class RenderSystem : public System<const Transform, const Camera>, public EventObserver
{
public:
	RenderSystem();
	~RenderSystem();

	virtual void Tick() override final;

	vk::RenderPass& GetRenderPass();
	class VulkanImage* GetRenderTexture();

	void OnSwapChainResize();

private:
	void CreateRenderResources();
	void DestroyRenderResources();

	void CreateRenderTextures();
	void CreateRenderPass();
	void CreateFrameBuffers();
	void CreatePipelines();

	void UpdateFrameBuffer();
	void UpdateObjectBuffer(const Transform& inTransform);

	vk::Framebuffer GetFrameBuffer() const;

private:
	class VulkanPipeline* myPipeline = nullptr;
	class VulkanPipeline* myFullscreenCopyPipeline = nullptr;

	vk::RenderPass myRenderPass;
	List<vk::Framebuffer> myFrameBuffers;

	vk::Framebuffer myRenderTextureFrameBuffer;
	vk::RenderPass myRenderTextureRenderPass;

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

	class VulkanTexture* myRenderTexture = nullptr;

	class Model* myModel = nullptr;
	class VulkanTexture* myTexture = nullptr;
};