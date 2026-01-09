#pragma once
#include "Engine/System/System.h"
#include "RenderGraph/RenderGraphTypes.hpp"

#if !SHIPPING
#define GPUMARK_SCOPE(inCommandBuffer, inString) inCommandBuffer.beginDebugUtilsLabelEXT(inString); ON_SCOPE_EXIT([inCommandBuffer](){ inCommandBuffer.endDebugUtilsLabelEXT(); })
#else
#define GPUMARK_SCOPE(inCommandBuffer, inString)
#endif


class VulkanCommandBuffer;
class ResizableBuffer;
class VulkanBuffer;
class TextureCube;
class IRenderPass;
class RenderGraph;

class RenderSystem : public System
{
    inline static RenderSystem* myInstance = nullptr;
public:
    inline static RenderSystem* Get() { return myInstance; }
    
    RenderSystem();
    ~RenderSystem();
    
    void Init() override final;
    void Tick();
    
    class VulkanImage* GetRenderTexture();
    class VulkanImage* GetResolvedRenderTexture() const;
    class VulkanImage* GetDepthTexture();
    class VulkanImage* GetResolvedDepthTexture() const;

    void OnSwapChainResize();

    float ReadDepthAtScreenPos(const glm::vec2& inNormalizedScreenPos) const;
    
    static VulkanCommandBuffer* CreateUploadCommandBuffer_TS();
    static void QueueCommandBufferForUpload_TS(VulkanCommandBuffer* inCommandBuffer, const List<ResourceUsage>& inResourceUsages);

    // TODO: Refactor these so they're not public like this.
    class VulkanImage* myDepthBuffer = nullptr;
    class VulkanImage* myRenderTexture = nullptr;

    VulkanBuffer* myCountBuffer = nullptr;
    
    TextureCube* myCubemap = nullptr;
    
private:
    void ExecuteRenderGraph(vk::CommandBuffer inCommandBuffer);
    
    void AddUploadPass(vk::CommandBuffer inCommandBuffer);

    void FlushUploadCommands();

private:
    void RegisterRenderResources();
    
    void CreateRenderResources();
    void DestroyRenderResources();

    void CreateRenderTextures();
    void CreateBuffers();
    
    void CreateRenderPasses();
    void DestroyRenderPasses();

private:
    struct UploadCommand
    {
        VulkanCommandBuffer* myCommandBuffer;
        List<ResourceUsage> myResourceUsage;
    };
    
    inline static std::recursive_mutex myUploadMutex;
    inline static List<UploadCommand> myQueuedUploadCommandBuffers;
    
    // This is a resolved render texture that has a MSAA count of 1. This is required so we can use it as input to imgui for the editor viewport. 
    class VulkanImage* myResolvedRenderTexture = nullptr;
    // This is used for editor depth reading as readback is not supported on multisampled textures.
    class VulkanImage* myResolvedDepthTexture = nullptr;
    
    RenderGraph* myRenderGraph;
};
