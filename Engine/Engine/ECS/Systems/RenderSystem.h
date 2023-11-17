#pragma once

#include "Engine/ECS/System.h"
#include "Engine/Events/EventObserver.h"
#include "Engine/ECS/Components/Transform.h"
#include "Engine/ECS/Components/Camera.h"
#include "Engine/ECS/Components/StaticMesh.h"
#include "Engine/Vulkan/VulkanUniformBuffer.hpp"

class RenderSystem : public System<const Transform, const Camera, const StaticMesh>, public EventObserver
{
public:
	RenderSystem();
	~RenderSystem();

	virtual void Tick() override final;

	vk::RenderPass& GetRenderPass();
	class VulkanImage* GetRenderTexture();

	void OnSwapChainResize();

private:
	void AddMeshPass(vk::CommandBuffer inCommandBuffer);
	void AddEditorPass(vk::CommandBuffer inCommandBuffer);

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

	/*
	* Index 0 is for main texture.
	* Index 1 is for depth texture.
	*/
	vk::ClearValue myClearValues[2] = {
		vk::ClearColorValue(std::array<float, 4>({ {0.1f, 0.1f, 0.1f, 1.0f} })),
		vk::ClearDepthStencilValue(1.0f, 0u) };

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