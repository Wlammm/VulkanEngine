#include "EnginePch.h"
#include "FoliagePrePass.h"

#include "Engine/Engine.h"
#include "Engine/Rendering/FoliageSystem.h"
#include "Engine/Shaders/Shared/FoliageStructs.hpp"
#include "Engine/Vulkan/VulkanBuffer.h"

#include <array>

void FoliagePrePass::PreExecute()
{
    IRenderPass::PreExecute();

    VulkanBuffer* indirectBuffer = Engine::GetEngineSystem<FoliageSystem>().GetIndirectBuffer();
    RegisterDynamicBufferUsage(indirectBuffer, vk::PipelineStageFlagBits::eTransfer, vk::AccessFlagBits::eTransferWrite);
}

void FoliagePrePass::Execute(vk::CommandBuffer inCommandBuffer)
{
    GPUMARK_SCOPE(inCommandBuffer, "FoliagePrePass");

    // Reset the per-LOD indirect commands: fixed vertexCount for the bin's blade
    // tessellation, instanceCount cleared to 0 for the cull pass to accumulate into.
    // firstInstance is 0 — the blade-buffer bin offset is passed via push constant in
    // FoliagePass instead, so we don't depend on SV_InstanceID including firstInstance.
    std::array<FoliageDrawCommand, FOLIAGE_LOD_COUNT> commands{};
    for (uint lod = 0; lod < FOLIAGE_LOD_COUNT; ++lod)
    {
        commands[lod].vertexCount = FOLIAGE_LOD_SEGMENTS[lod] * 2u + 1u;
        commands[lod].instanceCount = 0;
        commands[lod].firstVertex = 0;
        commands[lod].firstInstance = 0;
    }

    VulkanBuffer* indirectBuffer = Engine::GetEngineSystem<FoliageSystem>().GetIndirectBuffer();
    inCommandBuffer.updateBuffer(indirectBuffer->GetAPIResource(), 0,
        sizeof(FoliageDrawCommand) * FOLIAGE_LOD_COUNT, commands.data());
}
