#pragma once
#include "IRenderPass.h"
#include "Engine/AssetRegistry/AssetDefines.h"
#include "Engine/Vulkan/VulkanDescriptorSet.h"
#include "Engine/Vulkan/VulkanImage.h"

class Shader;
class VulkanImage;

struct TextureAttachmentDesc
{
    VulkanImage* myTexture;
    
    vk::AttachmentLoadOp myLoadOp;
    vk::AttachmentStoreOp myStoreOp;

    vk::ImageLayout myInitialLayout;
    vk::ImageLayout myFinalLayout;
};


class GraphicsPass : public IRenderPass
{
public:
    // If dynamic color attachments is set to true. The GetDynamicColorAttachments() and GetDynamicDepthAttachments() methods will be called before rendering starts to get the attachments for this frame.
    GraphicsPass(const SourcePath& inVertexShaderPath, const SourcePath& inFragmentShaderPath, const bool inDynamicAttachments = false, const bool inNeedsBindlessTexturesDescriptor = true);
    ~GraphicsPass();
    
    // Setup what textures should be rendered to.
    virtual void SetupAttachments() = 0;
    
    virtual List<vk::RenderingAttachmentInfo> GetDynamicColorAttachments() { check(false && "Dynamic passes needs to override this function"); return List<vk::RenderingAttachmentInfo>(); };
    virtual vk::RenderingAttachmentInfo GetDynamicDepthAttachments() { check(false && "Dynamic passes needs to override this function"); return vk::RenderingAttachmentInfo{}; }
    
    // Setup what buffers and textures should be bound to read from during this pass.
    virtual void SetupDescriptors() = 0;
    
    virtual vk::SampleCountFlagBits GetNumSamples() const;
    
    // Do the draw calls here.
    virtual void DrawCall(vk::CommandBuffer inCommandBuffer) = 0;
    
    void Execute(vk::CommandBuffer inCommandBuffer) override;
    
    void CreateResources() override;
    void DestroyResources() override;
    
    void OnShaderRecompiled();
    
    void AddDynamicColorAttachment(vk::Format inFormat, vk::ImageLayout inLayout, vk::AttachmentLoadOp inLoadOp, vk::AttachmentStoreOp inStoreOp, VulkanImage* myResolveImage = nullptr)
    {
        myColorAttachments.Emplace()
            .setLoadOp(inLoadOp)
            .setStoreOp(inStoreOp)
            .setImageLayout(inLayout)
            .setImageView(nullptr)
            .setClearValue(vk::ClearColorValue(std::array<float, 4>({ {0.1f, 0.1f, 0.1f, 1.0f} })));
        
        if (myResolveImage)
        {
            myColorAttachments.Last()
                .setResolveImageLayout(vk::ImageLayout::eColorAttachmentOptimal).
                setResolveImageView(myResolveImage->GetImageView())
                .setResolveMode(vk::ResolveModeFlagBits::eAverage);
        }
        
        myColorFormats.Add(inFormat);
    }
    
    void AddColorAttachment(VulkanImage* inImage, vk::ImageLayout inLayout, vk::AttachmentLoadOp inLoadOp, vk::AttachmentStoreOp inStoreOp, VulkanImage* myResolveImage = nullptr)
    {
        myColorAttachments.Emplace()
            .setLoadOp(inLoadOp)
            .setStoreOp(inStoreOp)
            .setImageLayout(inLayout)
            .setImageView(inImage->GetImageView())
            .setClearValue(vk::ClearColorValue(std::array<float, 4>({ {0.1f, 0.1f, 0.1f, 1.0f} })));
        
        if (myResolveImage)
        {
            myColorAttachments.Last()
                .setResolveImageLayout(vk::ImageLayout::eColorAttachmentOptimal).
                setResolveImageView(myResolveImage->GetImageView())
                .setResolveMode(vk::ResolveModeFlagBits::eAverage);
        }
        
        myColorFormats.Add(inImage->GetFormat());
    }
    
    void AddDepthAttachment(VulkanImage* inImage, vk::ImageLayout inLayout, vk::AttachmentLoadOp inLoadOp, vk::AttachmentStoreOp inStoreOp)
    {
        myHasDepthAttachment = true;
        myDepthAttachment
            .setLoadOp(inLoadOp)
            .setStoreOp(inStoreOp)
            .setImageLayout(inLayout)
            .setImageView(inImage->GetImageView())
            .setClearValue(vk::ClearDepthStencilValue(1.0f, 0u));
        
        myDepthFormat = inImage->GetFormat();
    }
    
protected:
    VulkanDescriptorSet myDescriptorSet;
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
    List<vk::Format> myColorFormats;
    
    vk::RenderingAttachmentInfo myDepthAttachment;
    vk::Format myDepthFormat;
};
