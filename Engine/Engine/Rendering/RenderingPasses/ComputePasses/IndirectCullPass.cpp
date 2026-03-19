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
    // vk::DrawIndexedIndirectCommand is a Vulkan struct not in the reflection system,
    // so it cannot be auto-bound and must be bound manually.
    BindBuffer(GPUResourceManager::Get()->GetBuffer<vk::DrawIndexedIndirectCommand>(), vk::ShaderStageFlagBits::eCompute, 2, vk::DescriptorType::eStorageBuffer, vk::AccessFlagBits::eShaderWrite);

    // The count buffer (binding 3) is a plain uint buffer not registered in GPUResourceManager,
    // so it must be bound manually after auto-binding.
    BindBuffer(RenderSystem::Get()->myCountBuffer, vk::ShaderStageFlagBits::eCompute, 3, vk::DescriptorType::eStorageBuffer, vk::AccessFlagBits::eShaderWrite);
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
			.setBuffer(GPUResourceManager::Get()->GetBuffer<vk::DrawIndexedIndirectCommand>()->GetBuffer()->GetAPIResource())
			.setOffset(0)
			.setSize(VK_WHOLE_SIZE),
		vk::BufferMemoryBarrier()
			.setSrcAccessMask(vk::AccessFlagBits::eShaderWrite)
			.setDstAccessMask(vk::AccessFlagBits::eIndirectCommandRead | vk::AccessFlagBits::eIndexRead)
			.setBuffer(GPUResourceManager::Get()->GetBuffer<PerDrawData>()->GetBuffer()->GetAPIResource())
			.setOffset(0)
			.setSize(VK_WHOLE_SIZE),
		vk::BufferMemoryBarrier()
			.setSrcAccessMask(vk::AccessFlagBits::eShaderWrite)
			.setDstAccessMask(vk::AccessFlagBits::eIndirectCommandRead)
			.setBuffer(RenderSystem::Get()->myCountBuffer->GetAPIResource())
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