#pragma once
#include "Engine/System/System.h"

#if DEBUG
#define GPUMARK_SCOPE(inCommandBuffer, inString) inCommandBuffer.beginDebugUtilsLabelEXT(inString); ON_SCOPE_EXIT([inCommandBuffer](){ inCommandBuffer.endDebugUtilsLabelEXT(); })
#else
#define GPUMARK_SCOPE(inCommandBuffer, inString)
#endif


class VulkanCommandBuffer;

class RenderSystem : public System
{
    inline static RenderSystem* myInstance = nullptr;
public:
    inline static RenderSystem* Get() { return myInstance; }
    
    RenderSystem();
    ~RenderSystem();
    
    void Init() override final;
    void Tick();

    vk::RenderPass& GetRenderPass();
    vk::RenderPass& GetImGuiRenderPass();
    
    class VulkanImage* GetRenderTexture();
    class VulkanImage* GetResolvedRenderTexture() const;
    class VulkanImage* GetDepthTexture();

    void OnSwapChainResize();

    float ReadDepthAtScreenPos(const glm::vec2& inNormalizedScreenPos) const;
    
    static VulkanCommandBuffer* CreateUploadCommandBuffer_TS();
    static void QueueCommandBufferForUpload_TS(VulkanCommandBuffer* commandBuffer);

    const class GDRPipeline& GetGDRPipeline() const;
    
    // TODO: Refactor these so they're not public like this.
    class VulkanImage* myDepthBuffer = nullptr;
    class VulkanImage* myRenderTexture = nullptr;

private:
    void AddGDRPass(vk::CommandBuffer inCommandBuffer);
    
    void AddUploadPass(vk::CommandBuffer inCommandBuffer);

    void FlushUploadCommands();

private:
    void CreateRenderResources();
    void DestroyRenderResources();

    void CreateRenderTextures();
    void CreateRenderPass();
    void CreateFrameBuffers();
    void CreatePipelines();
    
    vk::Framebuffer GetVkFrameBuffer() const;
    vk::Framebuffer GetVkCopyToSwapchainFrameBuffer() const;

private:
    inline static std::recursive_mutex myUploadMutex;
    inline static List<VulkanCommandBuffer*> myQueuedUploadCommandBuffers;
    
    bool myIsUsingGPUDrivenRendering = true;
    
    class GDRPipeline* myGDRPipeline = nullptr;

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

    
    // This is a resolved render texture that has a MSAA count of 1. This is required so we can use it as input to imgui for the editor viewport. 
    class VulkanImage* myResolvedRenderTexture = nullptr;
    // This is used for editor depth reading as readback is not supported on multisampled textures.
    class VulkanImage* myResolvedDepthTexture = nullptr;
};
