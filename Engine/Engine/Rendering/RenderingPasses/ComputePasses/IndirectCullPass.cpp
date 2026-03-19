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