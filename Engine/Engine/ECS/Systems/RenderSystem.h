#pragma once

#include "Engine/ECS/System.h"
#include "Engine/Events/EventObserver.h"
#include "Engine/Vulkan/VulkanUniformBuffer.hpp"
#include "Engine/Vulkan/VulkanStorageBufferFwd.hpp"

#if DEBUG
#define GPUMARK_SCOPE(inCommandBuffer, inString) inCommandBuffer.beginDebugUtilsLabelEXT(inString); ON_SCOPE_EXIT([inCommandBuffer](){ inCommandBuffer.endDebugUtilsLabelEXT(); })
#else
#define GPUMARK_SCOPE(inCommandBuffer, inString)
#endif

class RenderSystem : public System, public EventObserver
{
public:
	RenderSystem();
	~RenderSystem();

	void Init() override final;
	void Tick() override final;

	vk::RenderPass& GetRenderPass();
	class VulkanImage* GetRenderTexture();

	void OnSwapChainResize();

	static void AddUploadCommand_TS(void* inOwner, std::function<void(vk::CommandBuffer inCommandBuffer)> inFunction);
	static void RemoveUploadCommandsForOwner_TS(void* inOwner);
	static void FlushUploadCommands();

private:
	void AddUploadPass(vk::CommandBuffer inCommandBuffer);
	void AddMeshPass(vk::CommandBuffer inCommandBuffer);
	void AddShadowGenerationPass(vk::CommandBuffer inCommandBuffer);
	void AddFullscreenCopyPass(vk::CommandBuffer inCommandBuffer);

private:
	void CreateRenderResources();
	void DestroyRenderResources();

	void CreateRenderTextures();
	void CreateRenderPass();
	void CreateFrameBuffers();
	void CreatePipelines();

	//void UpdateFrameBuffer();
	//void UpdateObjectBuffer(const Transform& inTransform);

	vk::Framebuffer GetVkFrameBuffer() const;

private:
	inline static RenderSystem* myInstance = nullptr;

	struct UploadCommandData
	{
		void* myOwner;
		std::function<void(vk::CommandBuffer)> myFunction;
	};
	inline static std::mutex myUploadCommandsMutex;
	inline static List<UploadCommandData> myUploadCommands{};

	class MeshPipeline* myMeshPipeline = nullptr;
	class FullscreenPipeline* myCopyPipeline = nullptr;
	class ShadowPipeline* myShadowPipeline = nullptr;

	vk::RenderPass myRenderPass;
	vk::RenderPass myRenderTextureRenderPass;

	vk::Framebuffer myRenderTextureFrameBuffer;
	List<vk::Framebuffer> mySwapchainFrameBuffers;

	/*
	* Index 0 is for main texture.
	* Index 1 is for depth texture.
	*/
	vk::ClearValue myClearValues[2] = {
		vk::ClearColorValue(std::array<float, 4>({ {0.1f, 0.1f, 0.1f, 1.0f} })),
		vk::ClearDepthStencilValue(1.0f, 0u) };

	class VulkanImage* myDepthBuffer = nullptr;
	class VulkanImage* myRenderTexture = nullptr;
};