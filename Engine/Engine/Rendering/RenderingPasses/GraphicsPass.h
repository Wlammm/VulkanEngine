#pragma once
#include "IRenderPass.h"
#include "Engine/AssetRegistry/AssetDefines.h"
#include "Engine/Shaders/Shared/MeshStructs.hpp"
#include "Engine/Vulkan/VulkanDescriptorSet.h"
#include "Engine/Vulkan/VulkanImage.h"

class Shader;
class VulkanImage;

class GraphicsPass : public IRenderPass
{
public:
    // If dynamic color attachments is set to true. The GetDynamicColorAttachments() and GetDynamicDepthAttachments() methods will be called before rendering starts to get the attachments for this frame.
    GraphicsPass(const SourcePath& inVertexShaderPath, const SourcePath& inFragmentShaderPath, const bool inDynamicAttachments = false, const bool inNeedsBindlessTexturesDescriptor = true);
    ~GraphicsPass();
    
    // Setup what textures should be rendered to.
    virtual void SetupAttachments() = 0;
    
    virtual void GetDynamicColorAttachments() { check(false && "Dynamic passes needs to override this function"); };
    virtual void GetDynamicDepthAttachments() { check(false && "Dynamic passes needs to override this function"); }
    
    // Override to bind pass-specific resources that cannot be auto-bound (e.g. render textures,
    // combined image-samplers). Do NOT call Build() here — BuildDescriptors owns that.
    virtual void SetupDescriptors() override {}
    
    virtual vk::SampleCountFlagBits GetNumSamples() const;
    
    // Do the draw calls here.
    virtual void DrawCall(vk::CommandBuffer inCommandBuffer) = 0;
    
    virtual List<vk::VertexInputBindingDescription>& GetVertexBindingDescriptor() const;
    virtual List<vk::VertexInputAttributeDescription>& GetVertexAttributeDescriptions() const;
    virtual vk::PrimitiveTopology GetPrimitiveTopology() const;
    
    void Execute(vk::CommandBuffer inCommandBuffer) override;
    
    void PreExecute() override;
    
    void CreateResources() override;
    void DestroyResources() override;
    
    void OnShaderRecompiled();
    
    /*
     * Should be called inside SetupAttachments.
     */
    void AddColorAttachment(VulkanImage* inImage, vk::ImageLayout inLayout, vk::AttachmentLoadOp inLoadOp, vk::AttachmentStoreOp inStoreOp, VulkanImage* myResolveImage = nullptr);
    
    /*
     * Should be called inside GetDynamicColorAttachments.
     */
    void AddDynamicColorAttachment(VulkanImage* inImage, vk::ImageLayout inLayout, vk::AttachmentLoadOp inLoadOp, vk::AttachmentStoreOp inStoreOp);
    
    /*
     * Should be called inside SetupAttachments if this pass uses dynamic attachments.
     */
    void RegisterDynamicColorAttachment(vk::Format inFormat, vk::ImageLayout inLayout, vk::AttachmentLoadOp inLoadOp, vk::AttachmentStoreOp inStoreOp, VulkanImage* myResolveImage = nullptr);
    
    // Manual override for push constants — use only when auto-detection from SPIR-V is insufficient.
    template<typename Type>
    void SetPushConstantToType(vk::ShaderStageFlags inShaderStages)
    {
        myPushConstantRange.setSize(sizeof(Type)).setOffset(0).setStageFlags(inShaderStages);
    }
    
    /*
     * Should be called inside SetupAttachments.
     */
    void AddDepthAttachment(VulkanImage* inImage, vk::ImageLayout inLayout, vk::AttachmentLoadOp inLoadOp, vk::AttachmentStoreOp inStoreOp, VulkanImage* myDepthResolveImage = nullptr);
    
    
    void DrawToShadingBin(vk::CommandBuffer inCommandBuffer, const EShadingBin inShadingBin);
    
protected:
    vk::PipelineLayout myPipelineLayout;
    vk::Pipeline myPipeline;
    
    SharedPtr<Shader> myVertexShader;
    SharedPtr<Shader> myFragmentShader;
    
private:
    std::string myPassName = "";
    
    bool myHasDynamicAttachments = false;
    bool myHasDepthAttachment = false;
    bool myNeedsBindlessTextureDescriptor = true;
    
    List<vk::RenderingAttachmentInfo> myColorAttachments;
    List<vk::RenderingAttachmentInfo> myDynamicColorAttachments;
    List<vk::Format> myColorFormats;
    
    vk::RenderingAttachmentInfo myDepthAttachment;
    vk::RenderingAttachmentInfo myDynamicDepthAttachment;
    vk::Format myDepthFormat;
};
