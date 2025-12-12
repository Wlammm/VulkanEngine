#include "EnginePch.h"
#include "ComputePass.h"

#include "Engine/AssetRegistry/AssetRegistry.h"
#include "Engine/Assets/Shader.h"
#include "Engine/Vulkan/VulkanContext.h"
#include "Engine/Vulkan/VulkanDevice.h"

ComputePass::ComputePass(const SourcePath& inShaderPath)
{
    myShader = AssetRegistry::Get()->GetAssetSynchronous<Shader>(inShaderPath);
    myShader->OnShaderRecompiled.Bind(&ComputePass::OnShaderRecompiled, this);
}

ComputePass::~ComputePass()
{
    myShader->OnShaderRecompiled.UnBind(&ComputePass::OnShaderRecompiled, this);
}

void ComputePass::CreateResources()
{
    myDescriptorSet = {};
    SetupDescriptors();

    vk::DescriptorSetLayout descriptorSetLayout = myDescriptorSet.GetLayout();
    myPipelineLayout = VulkanContext::GetDevice()->createPipelineLayout(vk::PipelineLayoutCreateInfo().setSetLayouts(descriptorSetLayout));
    
    vk::ComputePipelineCreateInfo computePipelineCreateInfo{};
    computePipelineCreateInfo.setStage(vk::PipelineShaderStageCreateInfo()
        .setStage(vk::ShaderStageFlagBits::eCompute)
        .setModule(myShader->GetAPIResource())
        .setPName("main"));
    computePipelineCreateInfo.setLayout(myPipelineLayout);
    
    const vk::ResultValue<vk::Pipeline> result = VulkanContext::GetDevice()->createComputePipeline(VulkanContext::GetPipelineCache(), computePipelineCreateInfo);

    check(result.result == vk::Result::eSuccess);
    myPipeline = result.value;
}

void ComputePass::DestroyResources()
{
    VulkanContext::GetDevice()->destroyPipeline(myPipeline);
    VulkanContext::GetDevice()->destroyPipelineLayout(myPipelineLayout);
}

void ComputePass::Execute(vk::CommandBuffer inCommandBuffer)
{
    inCommandBuffer.bindPipeline(vk::PipelineBindPoint::eCompute, myPipeline);
    // TODO: Add support for multiple sets.
    inCommandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eCompute, myPipelineLayout, 0, myDescriptorSet.GetSet(), {});
    
    DispatchCall(inCommandBuffer);
}

void ComputePass::OnShaderRecompiled()
{
    DestroyResources();
    CreateResources();
}
