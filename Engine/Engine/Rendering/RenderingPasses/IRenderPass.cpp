#include "EnginePch.h"
#include "IRenderPass.h"

#include "Engine/Assets/Shader.h"
#include "Engine/Rendering/GPUResourceManager.h"
#include "Engine/Vulkan/VulkanUtils.hpp"

void IRenderPass::BindBuffer(
    class IGPUBuffer* inBuffer, 
    vk::ShaderStageFlags inShaderStages, 
    uint inBindingIndex,
    vk::DescriptorType inDescriptorType, 
    vk::AccessFlags inAccessFlags)
{
    myDescriptorSet.BindBuffer(inBuffer, inShaderStages, inBindingIndex, inDescriptorType);
    RegisterBufferUsage(inBuffer, PipelineFlagsFromShaderStages(inShaderStages), inAccessFlags);
}

void IRenderPass::BindSampler(vk::Sampler inSampler, vk::ShaderStageFlags inShaderStages, uint inBindingIndex)
{
    myDescriptorSet.BindSampler(inSampler, inShaderStages, inBindingIndex);
}

void IRenderPass::PreExecute()
{
    myDynamicResourceUsages.Clear();
}

void IRenderPass::BindAccelerationStructure(IGPUAccelerationStructure* inAS, vk::ShaderStageFlags inShaderStages, uint inBindingIndex,vk::AccessFlags inAccessFlags)
{
    myDescriptorSet.BindAccelerationStructure(inAS, inShaderStages, inBindingIndex);
    RegisterAccelerationStructureUsage(inAS, PipelineFlagsFromShaderStages(inShaderStages), inAccessFlags);
}

void IRenderPass::BindImage(
    class VulkanImage* inImage, 
    const vk::Sampler inSampler, 
    const uint inBinding,
    const vk::ShaderStageFlags inShaderFlags, 
    const vk::ImageLayout inImageLayout, 
    vk::AccessFlags inAccessFlags)
{
    myDescriptorSet.BindImage(inImage, inSampler, inBinding, inShaderFlags, inImageLayout);
    RegisterImageUsage(inImage, PipelineFlagsFromShaderStages(inShaderFlags), inAccessFlags, inImageLayout);
}

void IRenderPass::Build()
{
    myDescriptorSet.Build();
}

void IRenderPass::BuildDescriptors(const List<SharedPtr<Shader>>& inShaders)
{
    for (const SharedPtr<Shader>& shader : inShaders)
    {
        if (!shader)
            continue;
        
        for (const DescriptorSetInfo& setInfo : shader->GetDescriptorSetInfos())
        {
            if (setInfo.mySetIndex != 0)
                continue;  // Set 1+ are bindless/external — managed by TextureSystem

            for (const DescriptorBindingInfo& binding : setInfo.myBindings)
            {
                // Pass-specific image bindings are always handled manually in SetupDescriptors.
                if (binding.myDescriptorType == vk::DescriptorType::eCombinedImageSampler ||
                    binding.myDescriptorType == vk::DescriptorType::eSampledImage)
                    continue;

                // Pure sampler — resolve by HLSL variable name from the sampler registry.
                if (binding.myDescriptorType == vk::DescriptorType::eSampler)
                {
                    vk::Sampler sampler = VulkanUtils::TryGetSampler(binding.myName);
                    if (!sampler)
                    {
                        LOG_ERROR("Shader '%s': sampler binding %u '%s' not found in sampler registry. "
                                  "Rename the HLSL SamplerState to match a registered name "
                                  "(linearWrapSampler, linearClampSampler, pointWrapSampler, pointClampSampler).",
                                  shader->GetSourcePath().filename().string().c_str(),
                                  binding.myBindingIndex,
                                  binding.myName.c_str());
                    }
                    else
                    {
                        BindSampler(sampler, binding.myShaderStageFlags, binding.myBindingIndex);
                    }
                    continue;
                }

                GPUResourceManager* resourceManager = GPUResourceManager::Get();

                if (binding.myDescriptorType == vk::DescriptorType::eAccelerationStructureKHR)
                {
                    IGPUAccelerationStructure* as = resourceManager->TryGetAccelerationStructure(binding.myName);
                    if (!as)
                        as = resourceManager->TryGetAccelerationStructure(binding.myTypeName);

                    if (!as)
                    {
                        LOG_ERROR("Shader '%s': acceleration structure binding %u '%s' not found in GPUResourceManager.",
                                  shader->GetSourcePath().filename().string().c_str(),
                                  binding.myBindingIndex,
                                  binding.myName.c_str());
                        continue;
                    }
                    BindAccelerationStructure(as, binding.myShaderStageFlags, binding.myBindingIndex,vk::AccessFlagBits::eShaderRead);
                    continue;
                }

                const bool isBuffer = binding.myDescriptorType == vk::DescriptorType::eStorageBuffer
                                   || binding.myDescriptorType == vk::DescriptorType::eUniformBuffer;
                if (!isBuffer)
                    continue;

                IGPUBuffer* buffer = resourceManager->TryGetBuffer(binding.myTypeName);

                if (!buffer)
                    buffer = resourceManager->TryGetBufferByAlias(binding.myTypeName);

                if (!buffer)
                    buffer = resourceManager->TryGetBufferByAlias(binding.myName);

                if (!buffer)
                {
                    LOG_ERROR("Shader '%s': binding %u '%s' (type '%s') could not be auto-bound. "
                              "Register it in GPUResourceManager with a matching type alias or shader variable name.",
                              shader->GetSourcePath().filename().string().c_str(),
                              binding.myBindingIndex,
                              binding.myName.c_str(),
                              binding.myTypeName.c_str());
                    continue;
                }

                const vk::AccessFlags access = binding.myIsReadOnly
                    ? vk::AccessFlagBits::eShaderRead
                    : vk::AccessFlagBits::eShaderWrite;

                BindBuffer(buffer, binding.myShaderStageFlags, binding.myBindingIndex, binding.myDescriptorType, access);
            }
        }
    }

    // Auto-detect push constants from all shaders and merge stage flags for shared blocks.
    myPushConstantRange = vk::PushConstantRange{};
    for (const SharedPtr<Shader>& shader : inShaders)
    {
        if (!shader)
            continue;
        
        for (const PushConstantInfo& pc : shader->GetPushConstants())
        {
            if (myPushConstantRange.size == 0)
            {
                myPushConstantRange.setSize(pc.mySize).setOffset(pc.myOffset).setStageFlags(pc.myShaderStageFlags);
            }
            else if (myPushConstantRange.size == pc.mySize && myPushConstantRange.offset == pc.myOffset)
            {
                myPushConstantRange.stageFlags |= pc.myShaderStageFlags;
            }
            else
            {
                LOG_ERROR("Shader '%s': push constant block size/offset mismatch (expected size=%u offset=%u, got size=%u offset=%u). "
                          "All shaders in a pass must share the same push constant layout.",
                          shader->GetSourcePath().filename().string().c_str(),
                          myPushConstantRange.size, myPushConstantRange.offset,
                          pc.mySize, pc.myOffset);
            }
        }
    }

    SetupDescriptors();
    Build();
}

void IRenderPass::RegisterDynamicImageUsage(
    VulkanImage* inImage, 
    vk::PipelineStageFlags inStageFlags,
    vk::AccessFlags inAccess, 
    vk::ImageLayout inLayout)
{
    ResourceUsage& usage = myDynamicResourceUsages.Emplace();
    usage.SetToImage(inImage, inStageFlags, inAccess, inLayout);
}

void IRenderPass::RegisterImageUsage(
    VulkanImage* inImage, 
    vk::PipelineStageFlags inStageFlags,
    vk::AccessFlags inAccess, 
    vk::ImageLayout inLayout)
{
    ResourceUsage& usage = myResourceUsages.Emplace();
    usage.SetToImage(inImage, inStageFlags, inAccess, inLayout);    
}

void IRenderPass::RegisterBufferUsage(
    IGPUBuffer* inBuffer,
    vk::PipelineStageFlags inStages,
    vk::AccessFlags inAccessFlags)
{
    ResourceUsage& usage = myResourceUsages.Emplace();
    usage.SetToBuffer(inBuffer, inStages, inAccessFlags);
}

void IRenderPass::RegisterAccelerationStructureUsage(
    IGPUAccelerationStructure* inAS,
    vk::PipelineStageFlags inStages, 
    vk::AccessFlags inAccessFlags)
{
    ResourceUsage& usage = myResourceUsages.Emplace();
    usage.SetToAccelerationStructure(inAS, inStages, inAccessFlags);
}


vk::PipelineStageFlags IRenderPass::PipelineFlagsFromShaderStages(vk::ShaderStageFlags inShaderStages)
{
    vk::PipelineStageFlags flags;
        
    if (inShaderStages & vk::ShaderStageFlagBits::eVertex)
        flags |= vk::PipelineStageFlagBits::eVertexShader;
        
    if (inShaderStages & vk::ShaderStageFlagBits::eFragment)
        flags |= vk::PipelineStageFlagBits::eFragmentShader;
        
    if (inShaderStages & vk::ShaderStageFlagBits::eGeometry)
        flags |= vk::PipelineStageFlagBits::eGeometryShader;
        
    if (inShaderStages & vk::ShaderStageFlagBits::eCompute)
        flags |= vk::PipelineStageFlagBits::eComputeShader;
        
    return flags;
}