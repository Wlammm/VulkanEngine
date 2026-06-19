#include "EnginePch.h"
#include "FoliageCullPass.h"

#include "Engine/Engine.h"
#include "Engine/Rendering/FoliageSystem.h"

FoliageCullPass::FoliageCullPass()
    : ComputePass("Shaders/FoliageCullCS.hlsl")
{
}

void FoliageCullPass::DispatchCall(vk::CommandBuffer inCommandBuffer)
{
    const uint numInstances = Engine::GetEngineSystem<FoliageSystem>().GetNumInstances();
    if (numInstances == 0)
        return;

    inCommandBuffer.dispatch(static_cast<uint32_t>(std::ceil(numInstances / 256.0f)), 1, 1);
}
