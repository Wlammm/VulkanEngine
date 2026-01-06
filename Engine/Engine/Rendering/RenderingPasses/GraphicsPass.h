#pragma once
#include "IRenderPass.h"
#include "Engine/AssetRegistry/AssetDefines.h"
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
    
    // Setup what buffers and textures should be bound to read from during this pass.
    virtual void SetupDescriptors() = 0;
    
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
    
    void AddColorAttachment(VulkanImage* inImage, vk::ImageLayout inLayout, vk::AttachmentLoadOp inLoadOp, vk::AttachmentStoreOp inStoreOp, VulkanImage* myResolveImage = nullptr)
    {
        myColorAttachments.Emplace()
            .setLoadOp(inLoadOp)
            .setStoreOp(inStoreOp)
            .setImageLayout(inLayout)
            .setImageView(inImage->GetImageView())
            .setClearValue(vk::ClearColorValue(std::array<float, 4>({ {0.1f, 0.1f, 0.1f, 1.0f} })));
        
        RegisterImageUsage(inImage, vk::PipelineStageFlagBits::eColorAttachmentOutput, vk::AccessFlagBits::eColorAttachmentWrite, inLayout);
        
        if (myResolveImage)
        {
            myColorAttachments.Last()
                .setResolveImageLayout(vk::ImageLayout::eColorAttachmentOptimal).
                setResolveImageView(myResolveImage->GetImageView())
                .setResolveMode(vk::ResolveModeFlagBits::eAverage);
            
            RegisterImageUsage(myResolveImage, vk::PipelineStageFlagBits::eColorAttachmentOutput, vk::AccessFlagBits::eColorAttachmentWrite, vk::ImageLayout::eColorAttachmentOptimal);
        }
        
        myColorFormats.Add(inImage->GetFormat());
    }
    
    /*
     * Should be called inside GetDynamicColorAttachments.
     */
    void AddDynamicColorAttachment(VulkanImage* inImage, vk::ImageLayout inLayout, vk::AttachmentLoadOp inLoadOp, vk::AttachmentStoreOp inStoreOp)
    {
        myDynamicColorAttachments.Add(vk::RenderingAttachmentInfo().setLoadOp(vk::AttachmentLoadOp::eDontCare)
            .setImageLayout(inLayout)
            .setLoadOp(inLoadOp)
            .setStoreOp(inStoreOp)
            .setImageView(inImage->GetImageView())
            .setClearValue(vk::ClearColorValue(std::array<float, 4>({ {0.1f, 0.1f, 0.1f, 1.0f} }))));  
        
        RegisterImageUsage(inImage, vk::PipelineStageFlagBits::eColorAttachmentOutput, vk::AccessFlagBits::eColorAttachmentWrite, inLayout);
    }
    
    /*
     * Should be called inside SetupAttachments if this pass uses dynamic attachments.
     */
    void RegisterDynamicColorAttachment(vk::Format inFormat, vk::ImageLayout inLayout, vk::AttachmentLoadOp inLoadOp, vk::AttachmentStoreOp inStoreOp, VulkanImage* myResolveImage = nullptr)
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
                .setResolveImageLayout(vk::ImageLayout::eColorAttachmentOptimal)
                .setResolveImageView(myResolveImage->GetImageView())
                .setResolveMode(vk::ResolveModeFlagBits::eAverage);
        }
        
        myColorFormats.Add(inFormat);
    }
    
    template<typename Type>
    void SetPushConstantToType(vk::ShaderStageFlags inShaderStages)
    {
        myPushConstantRange.setSize(sizeof(Type)).setOffset(0).setStageFlags(inShaderStages);
    }
    
    void AddDepthAttachment(VulkanImage* inImage, vk::ImageLayout inLayout, vk::AttachmentLoadOp inLoadOp, vk::AttachmentStoreOp inStoreOp, VulkanImage* myDepthResolveImage = nullptr)
    {
        myHasDepthAttachment = true;
        myDepthAttachment
            .setLoadOp(inLoadOp)
            .setStoreOp(inStoreOp)
            .setImageLayout(inLayout)
            .setImageView(inImage->GetImageView())
            .setClearValue(vk::ClearDepthStencilValue(1.0f, 0u));
        
        RegisterImageUsage(inImage, 
            vk::PipelineStageFlagBits::eEarlyFragmentTests | vk::PipelineStageFlagBits::eLateFragmentTests, 
            vk::AccessFlagBits::eDepthStencilAttachmentWrite | vk::AccessFlagBits::eDepthStencilAttachmentRead, 
            inLayout);
        
        if (myDepthResolveImage)
        {
            myDepthAttachment
                .setResolveImageLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal)
                .setResolveImageView(myDepthResolveImage->GetImageView())
                .setResolveMode(vk::ResolveModeFlagBits::eAverage);
            
            RegisterImageUsage(myDepthResolveImage, 
                vk::PipelineStageFlagBits::eEarlyFragmentTests | vk::PipelineStageFlagBits::eLateFragmentTests, 
                vk::AccessFlagBits::eDepthStencilAttachmentWrite | vk::AccessFlagBits::eDepthStencilAttachmentRead, 
                inLayout);
        }
        
        myDepthFormat = inImage->GetFormat();
    }
    
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
    
    vk::PushConstantRange myPushConstantRange{};
    
    vk::RenderingAttachmentInfo myDepthAttachment;
    vk::RenderingAttachmentInfo myDynamicDepthAttachment;
    vk::Format myDepthFormat;
};
