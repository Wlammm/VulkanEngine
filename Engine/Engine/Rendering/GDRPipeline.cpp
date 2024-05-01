#include "EnginePch.h"
#include "GDRPipeline.h"

#include "Engine.h"
#include "Assets/AssetRegistry.h"
#include "Vulkan/VulkanAllocator.h"
#include "Vulkan/VulkanBuffer.h"
#include "Vulkan/VulkanContext.h"
#include "Vulkan/VulkanDevice.h"
#include "Vulkan/VulkanShader.h"

GDRPipeline::GDRPipeline()
{
    myCullShader = Engine::GetAssetRegistry().GetShader("IndirectCulling.comp");
    
    CreateDescriptorSets();
    CreatePipeline();
}

GDRPipeline::~GDRPipeline()
{
    VulkanContext::GetDevice()->destroyPipelineLayout(myPipelineLayout);
    VulkanContext::GetDevice()->destroyPipeline(myPipeline);
    VulkanAllocator::DestroyBuffer_TS(myTestDataBuffer);
}

void GDRPipeline::AddCommands(vk::CommandBuffer inCommandBuffer)
{
    inCommandBuffer.bindPipeline(vk::PipelineBindPoint::eCompute, myPipeline);
    //inCommandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eCompute, myPipelineLayout, 0, myCullDescriptorSet.GetSet(), {});
    inCommandBuffer.dispatch(1, 1, 1);
}

void GDRPipeline::CreateDescriptorSets()
{
    myTestDataBuffer = VulkanAllocator::AllocateBuffer_TS("TestBuffer", VulkanBuffer::StorageBufferCreateInfo(sizeof(TestBuffer)),VMA_MEMORY_USAGE_AUTO);
    
    myCullDescriptorSet.BindBuffer(myTestDataBuffer, vk::ShaderStageFlagBits::eCompute, 0, vk::DescriptorType::eStorageBuffer);
    myCullDescriptorSet.Build();
}

void GDRPipeline::CreatePipeline()
{
    const List<vk::DescriptorSetLayout> layouts{ /*myCullDescriptorSet.GetLayout()*/ };
    myPipelineLayout = VulkanContext::GetDevice()->createPipelineLayout(vk::PipelineLayoutCreateInfo().setSetLayouts(layouts));
    
    vk::ComputePipelineCreateInfo createInfo = vk::ComputePipelineCreateInfo();
    createInfo.setStage(vk::PipelineShaderStageCreateInfo().setStage(vk::ShaderStageFlagBits::eCompute).setModule(*myCullShader).setPName("main"));
    createInfo.setLayout(myPipelineLayout);

    const vk::ResultValue<vk::Pipeline> result = VulkanContext::GetDevice()->createComputePipeline(VulkanContext::GetPipelineCache(), createInfo);

    check(result.result == vk::Result::eSuccess);
    myPipeline = result.value;
}
