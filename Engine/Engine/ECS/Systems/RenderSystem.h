#pragma once

#include "Engine/ECS/System.h"
#include "Engine/Events/EventObserver.h"
#include "Engine/ECS/Components/Transform.h"
#include "Engine/ECS/Components/Camera.h"
#include "Engine/ECS/Components/StaticMesh.h"
#include "Engine/ECS/Components/PointLight.h"
#include "Engine/Vulkan/VulkanUniformBuffer.hpp"
#include "Engine/Vulkan/VulkanStorageBuffer.hpp"

class RenderSystem : public System<const Transform, const Camera, const StaticMesh, const PointLight>, public EventObserver
{
public:
	RenderSystem();
	~RenderSystem();

	void Init();

	virtual void Tick() override final;

	vk::RenderPass& GetRenderPass();
	class VulkanImage* GetRenderTexture();

	const IVulkanUniformBuffer& GetFrameBuffer() const;
	const IVulkanUniformBuffer& GetObjectBuffer() const;

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

	void BuildPointLightBuffer();

	void UpdateFrameBuffer();
	void UpdateObjectBuffer(const Transform& inTransform);

	vk::Framebuffer GetVkFrameBuffer() const;

private:
	class VulkanPipeline* myPipeline = nullptr;
	class VulkanPipeline* myFullscreenCopyPipeline = nullptr;

	class MeshPipeline* myMeshPipeline = nullptr;

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

	struct PointLightData
	{
		Color myColor;
		Vec3f myPosition;
		float myRange;
	};
	VulkanStorageBuffer<PointLightData> myPointLightData{ vk::ShaderStageFlagBits::eFragment, 3 };

	class VulkanDepthBuffer* myDepthBuffer = nullptr;

	class VulkanTexture* myRenderTexture = nullptr;
};