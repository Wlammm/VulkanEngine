#include "EnginePch.h"
#include "GDRPipeline.h"

#include "Engine.h"
#include "Assets/AssetRegistry.h"
#include "Vulkan/ObjectSystem.h"
#include "Vulkan/ResizableBuffer.h"
#include "Vulkan/VulkanAllocator.h"
#include "Vulkan/VulkanBuffer.h"
#include "Vulkan/VulkanContext.h"
#include "Vulkan/VulkanDevice.h"
#include "Vulkan/VulkanShader.h"

GDRPipeline::GDRPipeline()
{
    myCullShader = Engine::GetAssetRegistry().GetShader("IndirectCulling.comp");

    CreateBuffers();
    CreateDescriptorSets();
    CreatePipeline();
}

GDRPipeline::~GDRPipeline()
{
    del(myIndirectCommandsBuffer);
    
    VulkanAllocator::DestroyBuffer_TS(myCountBuffer);
    myCountBuffer = nullptr;
    
    VulkanContext::GetDevice()->destroyPipelineLayout(myPipelineLayout);
    VulkanContext::GetDevice()->destroyPipeline(myPipeline);
}

void GDRPipeline::AddCommands(vk::CommandBuffer inCommandBuffer)
{
    inCommandBuffer.bindPipeline(vk::PipelineBindPoint::eCompute, myPipeline);
    inCommandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eCompute, myPipelineLayout, 0, myDescriptorSet.GetSet(), {});
    inCommandBuffer.dispatch(1, 1, 1);
}

void GDRPipeline::CreateDescriptorSets()
{
    MeshSystem& meshSystem = Engine::GetEngineSystem<MeshSystem>();
    ObjectSystem& objectSystem = Engine::GetEngineSystem<ObjectSystem>();

    myDescriptorSet.BindBuffer(meshSystem.GetBuffer(), vk::ShaderStageFlagBits::eCompute, 0, vk::DescriptorType::eStorageBuffer);
    myDescriptorSet.BindBuffer(objectSystem.GetBuffer(), vk::ShaderStageFlagBits::eCompute, 1, vk::DescriptorType::eStorageBuffer);
    myDescriptorSet.BindBuffer(myIndirectCommandsBuffer->GetBuffer(), vk::ShaderStageFlagBits::eCompute, 2, vk::DescriptorType::eStorageBuffer);
    myDescriptorSet.BindBuffer(myCountBuffer, vk::ShaderStageFlagBits::eCompute, 3, vk::DescriptorType::eStorageBuffer);
    myDescriptorSet.Build();
}

void GDRPipeline::CreatePipeline()
{
    const List<vk::DescriptorSetLayout> layouts{ myDescriptorSet.GetLayout() };
    myPipelineLayout = VulkanContext::GetDevice()->createPipelineLayout(vk::PipelineLayoutCreateInfo().setSetLayouts(layouts));
    
    vk::ComputePipelineCreateInfo createInfo = vk::ComputePipelineCreateInfo();
    createInfo.setStage(vk::PipelineShaderStageCreateInfo().setStage(vk::ShaderStageFlagBits::eCompute).setModule(*myCullShader).setPName("main"));
    createInfo.setLayout(myPipelineLayout);

    const vk::ResultValue<vk::Pipeline> result = VulkanContext::GetDevice()->createComputePipeline(VulkanContext::GetPipelineCache(), createInfo);

    check(result.result == vk::Result::eSuccess);
    myPipeline = result.value;
}

void GDRPipeline::CreateBuffers()
{
    const ObjectSystem& objectSystem = Engine::GetEngineSystem<ObjectSystem>();
    
    myCountBuffer = VulkanAllocator::AllocateBuffer_TS("IndirectDrawCount Buffer",
        vk::BufferCreateInfo()
        .setSize(sizeof(uint))
        .setUsage(vk::BufferUsageFlagBits::eIndirectBuffer | vk::BufferUsageFlagBits::eStorageBuffer),
        VMA_MEMORY_USAGE_AUTO);


    const uint numObjects = objectSystem.GetNumObjects() != 0 ? objectSystem.GetNumObjects() : 4;
    
    myIndirectCommandsBuffer = new ResizableBuffer(VulkanAllocator::AllocateBuffer_TS("IndirectDrawCommands Buffer",
        vk::BufferCreateInfo()
        .setSize(sizeof(vk::DrawIndexedIndirectCommand) * numObjects)
        .setUsage(vk::BufferUsageFlagBits::eIndirectBuffer | vk::BufferUsageFlagBits::eStorageBuffer),
        VMA_MEMORY_USAGE_AUTO));
}
