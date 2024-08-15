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
    myPrePassShader = Engine::GetAssetRegistry().GetShader("PrePass.comp");

    CreateBuffers();
    CreatePrePassResources();
    CreateCullPassResources();
}

GDRPipeline::~GDRPipeline()
{
    myPrePass.Destroy();
    myCullPass.Destroy();
    
    del(myIndirectCommandsBuffer);
    VulkanAllocator::DestroyBuffer_TS(myCountBuffer);
    myCountBuffer = nullptr;
}

void GDRPipeline::AddCommands(vk::CommandBuffer inCommandBuffer)
{
    ObjectSystem& objectSystem = Engine::GetEngineSystem<ObjectSystem>();
    size_t requiredSize = objectSystem.GetNumObjects() * sizeof(vk::DrawIndexedIndirectCommand);
    if(requiredSize > myIndirectCommandsBuffer->GetBuffer()->GetSize())
    {
        myIndirectCommandsBuffer->Resize(requiredSize);
    }
    
    ExecuteComputePass(inCommandBuffer, myPrePass);
    ExecuteComputePass(inCommandBuffer, myCullPass);

    
}
VulkanBuffer* GDRPipeline::GetCountBuffer() const
{
    return myCountBuffer;
}

VulkanBuffer* GDRPipeline::GetIndirectBuffer() const
{
    return myIndirectCommandsBuffer->GetBuffer();    
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
        .setUsage(vk::BufferUsageFlagBits::eIndirectBuffer | vk::BufferUsageFlagBits::eStorageBuffer | vk::BufferUsageFlagBits::eTransferSrc | vk::BufferUsageFlagBits::eTransferDst),
        VMA_MEMORY_USAGE_AUTO));
}

void GDRPipeline::ComputePassResources::Destroy()
{
    VulkanContext::GetDevice()->destroyPipelineLayout(myPipelineLayout);
    VulkanContext::GetDevice()->destroyPipeline(myPipeline);
    del(myDescriptorSet);
}

void GDRPipeline::ExecuteComputePass(vk::CommandBuffer inCommandBuffer, const ComputePassResources& inComputePassResources)
{
    inCommandBuffer.bindPipeline(vk::PipelineBindPoint::eCompute, inComputePassResources.myPipeline);
    inCommandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eCompute, inComputePassResources.myPipelineLayout, 0, inComputePassResources.myDescriptorSet->GetSet(), {});
    inCommandBuffer.dispatch(1, 1, 1);
}

void GDRPipeline::CreatePrePassResources()
{
    // Create descriptors
    MeshSystem& meshSystem = Engine::GetEngineSystem<MeshSystem>();
    ObjectSystem& objectSystem = Engine::GetEngineSystem<ObjectSystem>();
    
    myPrePass.myDescriptorSet = new VulkanDescriptorSet();
    myPrePass.myDescriptorSet->BindBuffer(myCountBuffer, vk::ShaderStageFlagBits::eCompute, 0, vk::DescriptorType::eStorageBuffer);
    myPrePass.myDescriptorSet->Build();

    // Create pipeline
    const List<vk::DescriptorSetLayout> layouts{ myPrePass.myDescriptorSet->GetLayout() };
    myPrePass.myPipelineLayout = VulkanContext::GetDevice()->createPipelineLayout(vk::PipelineLayoutCreateInfo().setSetLayouts(layouts));
    
    vk::ComputePipelineCreateInfo createInfo = vk::ComputePipelineCreateInfo();
    createInfo.setStage(vk::PipelineShaderStageCreateInfo().setStage(vk::ShaderStageFlagBits::eCompute).setModule(*myPrePassShader).setPName("main"));
    createInfo.setLayout(myPrePass.myPipelineLayout);

    const vk::ResultValue<vk::Pipeline> result = VulkanContext::GetDevice()->createComputePipeline(VulkanContext::GetPipelineCache(), createInfo);

    check(result.result == vk::Result::eSuccess);
    myPrePass.myPipeline = result.value;
}

void GDRPipeline::CreateCullPassResources()
{
    // Create descriptors
    MeshSystem& meshSystem = Engine::GetEngineSystem<MeshSystem>();
    ObjectSystem& objectSystem = Engine::GetEngineSystem<ObjectSystem>();

    myCullPass.myDescriptorSet = new VulkanDescriptorSet();
    myCullPass.myDescriptorSet->BindBuffer(meshSystem.GetBuffer(), vk::ShaderStageFlagBits::eCompute, 0, vk::DescriptorType::eStorageBuffer);
    myCullPass.myDescriptorSet->BindBuffer(objectSystem.GetBuffer(), vk::ShaderStageFlagBits::eCompute, 1, vk::DescriptorType::eStorageBuffer);
    myCullPass.myDescriptorSet->BindBuffer(myIndirectCommandsBuffer, vk::ShaderStageFlagBits::eCompute, 2, vk::DescriptorType::eStorageBuffer);
    myCullPass.myDescriptorSet->BindBuffer(myCountBuffer, vk::ShaderStageFlagBits::eCompute, 3, vk::DescriptorType::eStorageBuffer);
    myCullPass.myDescriptorSet->Build();

    // Create pipeline
    const List<vk::DescriptorSetLayout> layouts{ myCullPass.myDescriptorSet->GetLayout() };
    myCullPass.myPipelineLayout = VulkanContext::GetDevice()->createPipelineLayout(vk::PipelineLayoutCreateInfo().setSetLayouts(layouts));
    
    vk::ComputePipelineCreateInfo createInfo = vk::ComputePipelineCreateInfo();
    createInfo.setStage(vk::PipelineShaderStageCreateInfo().setStage(vk::ShaderStageFlagBits::eCompute).setModule(*myCullShader).setPName("main"));
    createInfo.setLayout(myCullPass.myPipelineLayout);

    const vk::ResultValue<vk::Pipeline> result = VulkanContext::GetDevice()->createComputePipeline(VulkanContext::GetPipelineCache(), createInfo);

    check(result.result == vk::Result::eSuccess);
    myCullPass.myPipeline = result.value;
}
