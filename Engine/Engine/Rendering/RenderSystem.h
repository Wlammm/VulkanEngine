#pragma once
#include "Engine/Events/EventObserver.h"
#include "Engine/System/System.h"

#if DEBUG
#define GPUMARK_SCOPE(inCommandBuffer, inString) inCommandBuffer.beginDebugUtilsLabelEXT(inString); ON_SCOPE_EXIT([inCommandBuffer](){ inCommandBuffer.endDebugUtilsLabelEXT(); })
#else
#define GPUMARK_SCOPE(inCommandBuffer, inString)
#endif


class VulkanCommandBuffer;

class RenderSystem : public System, public EventObserver
{
public:
    RenderSystem();
    ~RenderSystem();
    
    void Init() override final;
    void Tick();

    vk::RenderPass& GetRenderPass();
    vk::RenderPass& GetImGuiRenderPass();
    
    class VulkanImage* GetRenderTexture();
    class VulkanImage* GetResolvedRenderTexture() const;
    class VulkanImage* GetDepthTexture();
    class VulkanImage* GetDirectionalLightShadowMap() const;
    
    void OnSwapChainResize();
	
    static VulkanCommandBuffer* CreateUploadCommandBuffer_TS();
    static void QueueCommandBufferForUpload_TS(VulkanCommandBuffer* commandBuffer);

    const class GDRPipeline& GetGDRPipeline() const;

private:
    void AddGDRPass(vk::CommandBuffer inCommandBuffer);
    
    void AddUploadPass(vk::CommandBuffer inCommandBuffer);
    void AddDebugPass(vk::CommandBuffer inCommandBuffer);
    void AddFullscreenCopyPass(vk::CommandBuffer inCommandBuffer);
    void AddImGuiPass(vk::CommandBuffer inCommandBuffer);

    void FlushUploadCommands();

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
    vk::Framebuffer GetVkCopyToSwapchainFrameBuffer() const;

private:
    inline static std::recursive_mutex myUploadMutex;
    inline static List<VulkanCommandBuffer*> myQueuedUploadCommandBuffers;
    
    bool myIsUsingGPUDrivenRendering = true;
    
    class FullscreenPipeline* myCopyPipeline = nullptr;
    class DebugPipeline* myDebugPipeline = nullptr;
    class GDRPipeline* myGDRPipeline = nullptr;
    class SkyboxPipeline* mySkyboxPipeline = nullptr;

    vk::RenderPass myRenderPass;
    vk::RenderPass myCopyToSwapchainRenderPass;

    vk::Framebuffer myVkFrameBuffer;
    List<vk::Framebuffer> myCopyToSwapchainFrameBuffers;

    /*
    * Index 0 is for main texture.
    * Index 1 is for depth texture.
    */
    vk::ClearValue myClearValues[2] = {
        vk::ClearColorValue(std::array<float, 4>({ {0.1f, 0.1f, 0.1f, 1.0f} })),
        vk::ClearDepthStencilValue(1.0f, 0u) };

    class VulkanImage* myDepthBuffer = nullptr;
    class VulkanImage* myRenderTexture = nullptr;
    
    // This is a resolved render texture that has a MSAA count of 1. This is required so we can use it as input to imgui for the editor viewport. 
    class VulkanImage* myResolvedRenderTexture = nullptr;

    static constexpr glm::vec2 myShadowMapSize = glm::vec2(4096, 4096);
    class VulkanImage* myDirectionalLightShadowMap = nullptr;
};
