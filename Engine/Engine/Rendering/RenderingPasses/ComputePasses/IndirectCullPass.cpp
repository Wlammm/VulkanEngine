#include "EnginePch.h"
#include "IndirectCullPass.h"

#include "Engine/Vulkan/GPUSceneSystem.h"

IndirectCullPass::IndirectCullPass()
    : ComputePass("Shaders/IndirectCullingCS.hlsl")
{
}


void IndirectCullPass::DispatchCall(vk::CommandBuffer inCommandBuffer)
{
    GPUSceneSystem& objectSystem = Engine::GetEngineSystem<GPUSceneSystem>();

    const uint numObjects = objectSystem.GetNumObjects();
    if (numObjects == 0)
        return;

    inCommandBuffer.dispatch(static_cast<uint32_t>(std::ceil(numObjects / 256.0f)), 1, 1);
}