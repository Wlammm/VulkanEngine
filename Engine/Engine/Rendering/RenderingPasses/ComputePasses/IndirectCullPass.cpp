#include "EnginePch.h"
#include "IndirectCullPass.h"

#include "Engine/Rendering/GPUResourceManager.h"
#include "Engine/Rendering/IndexBufferSystem.h"
#include "Engine/Rendering/MeshSystem.h"
#include "Engine/Rendering/VertexBufferSystem.h"
#include "Engine/Vulkan/GPUSceneSystem.h"

IndirectCullPass::IndirectCullPass()
    : ComputePass("Shaders/IndirectCullingCS.hlsl")
{
}

void IndirectCullPass::SetupDescriptors()
{
    MeshSystem& meshSystem = Engine::GetEngineSystem<MeshSystem>();
    GPUSceneSystem& objectSystem = Engine::GetEngineSystem<GPUSceneSystem>();
    VertexBufferSystem& vertexSystem = Engine::GetEngineSystem<VertexBufferSystem>();
    IndexBufferSystem& indexSystem = Engine::GetEngineSystem<IndexBufferSystem>();
    
    myDescriptorSet.BindBuffer(meshSystem.GetBuffer(), vk::ShaderStageFlagBits::eCompute, 0, vk::DescriptorType::eStorageBuffer);
    myDescriptorSet.BindBuffer(vertexSystem.GetGlobalSparseVertexDataBuffer(), vk::ShaderStageFlagBits::eCompute, 6, vk::DescriptorType::eStorageBuffer);
    myDescriptorSet.BindBuffer(indexSystem.GetGlobalSparseIndexDataBuffer(), vk::ShaderStageFlagBits::eCompute, 7, vk::DescriptorType::eStorageBuffer);
    myDescriptorSet.BindBuffer(objectSystem.GetBuffer(), vk::ShaderStageFlagBits::eCompute, 1, vk::DescriptorType::eStorageBuffer);
    myDescriptorSet.BindBuffer(RenderSystem::Get()->myIndirectCommandsBuffer, vk::ShaderStageFlagBits::eCompute, 2, vk::DescriptorType::eStorageBuffer);
    myDescriptorSet.BindBuffer(RenderSystem::Get()->myCountBuffer, vk::ShaderStageFlagBits::eCompute, 3, vk::DescriptorType::eStorageBuffer);
    myDescriptorSet.BindBuffer(RenderSystem::Get()->myPerDrawDataBuffer, vk::ShaderStageFlagBits::eCompute, 4, vk::DescriptorType::eStorageBuffer);
    myDescriptorSet.BindBuffer(RenderSystem::Get()->myPerDrawDataNoDepthBuffer, vk::ShaderStageFlagBits::eCompute, 8, vk::DescriptorType::eStorageBuffer);
    myDescriptorSet.BindBuffer(RenderSystem::Get()->myCountNoDepthBuffer, vk::ShaderStageFlagBits::eCompute, 9, vk::DescriptorType::eStorageBuffer);
    myDescriptorSet.BindBuffer(RenderSystem::Get()->myIndirectCommandsBufferNoDepth, vk::ShaderStageFlagBits::eCompute, 10, vk::DescriptorType::eStorageBuffer);
	myDescriptorSet.BindBuffer(GPUResourceManager::Get()->GetBuffer<SceneHeader>(), vk::ShaderStageFlagBits::eCompute, 11, vk::DescriptorType::eUniformBuffer);
    myDescriptorSet.Build();
}

void IndirectCullPass::DispatchCall(vk::CommandBuffer inCommandBuffer)
{
    GPUSceneSystem& objectSystem = Engine::GetEngineSystem<GPUSceneSystem>();
	
    inCommandBuffer.dispatch(static_cast<uint32_t>(std::ceil(objectSystem.GetNumObjects() / 256.0f)), 1, 1);
    
    // Memory barrier to ensure the compute shader writes are visible to subsequent draw calls
	vk::BufferMemoryBarrier bufferMemoryBarriers[] = {
		vk::BufferMemoryBarrier()
			.setSrcAccessMask(vk::AccessFlagBits::eShaderWrite)
			.setDstAccessMask(vk::AccessFlagBits::eIndirectCommandRead | vk::AccessFlagBits::eVertexAttributeRead)
			.setBuffer(RenderSystem::Get()->myIndirectCommandsBuffer->GetBuffer()->GetAPIResource())
			.setOffset(0)
			.setSize(VK_WHOLE_SIZE),
		vk::BufferMemoryBarrier()
			.setSrcAccessMask(vk::AccessFlagBits::eShaderWrite)
			.setDstAccessMask(vk::AccessFlagBits::eIndirectCommandRead | vk::AccessFlagBits::eVertexAttributeRead)
			.setBuffer(RenderSystem::Get()->myIndirectCommandsBufferNoDepth->GetBuffer()->GetAPIResource())
			.setOffset(0)
			.setSize(VK_WHOLE_SIZE),
		vk::BufferMemoryBarrier()
			.setSrcAccessMask(vk::AccessFlagBits::eShaderWrite)
			.setDstAccessMask(vk::AccessFlagBits::eIndirectCommandRead | vk::AccessFlagBits::eIndexRead)
			.setBuffer(RenderSystem::Get()->myPerDrawDataBuffer->GetBuffer()->GetAPIResource())
			.setOffset(0)
			.setSize(VK_WHOLE_SIZE),
		vk::BufferMemoryBarrier()
			.setSrcAccessMask(vk::AccessFlagBits::eShaderWrite)
			.setDstAccessMask(vk::AccessFlagBits::eIndirectCommandRead | vk::AccessFlagBits::eIndexRead)
			.setBuffer(RenderSystem::Get()->myPerDrawDataNoDepthBuffer->GetBuffer()->GetAPIResource())
			.setOffset(0)
			.setSize(VK_WHOLE_SIZE),
		vk::BufferMemoryBarrier()
			.setSrcAccessMask(vk::AccessFlagBits::eShaderWrite)
			.setDstAccessMask(vk::AccessFlagBits::eIndirectCommandRead)
			.setBuffer(RenderSystem::Get()->myCountBuffer->GetAPIResource())
			.setOffset(0)
			.setSize(VK_WHOLE_SIZE),
		vk::BufferMemoryBarrier()
			.setSrcAccessMask(vk::AccessFlagBits::eShaderWrite)
			.setDstAccessMask(vk::AccessFlagBits::eIndirectCommandRead)
			.setBuffer(RenderSystem::Get()->myCountNoDepthBuffer->GetAPIResource())
			.setOffset(0)
			.setSize(VK_WHOLE_SIZE),
	};

	inCommandBuffer.pipelineBarrier(
		vk::PipelineStageFlagBits::eComputeShader,
		vk::PipelineStageFlagBits::eDrawIndirect | vk::PipelineStageFlagBits::eVertexInput,
		vk::DependencyFlags{},
		nullptr,
		bufferMemoryBarriers,
		nullptr);
}