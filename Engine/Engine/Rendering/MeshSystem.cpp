#include "EnginePch.h"
#include "MeshSystem.h"

#include "Vulkan/VulkanAllocator.h"

MeshSystem::MeshSystem()
    : myRenderItems{"RenderItems Buffer", vk::BufferCreateInfo().setUsage(vk::BufferUsageFlagBits::eStorageBuffer)}
{
    SetDependencies<>();
}

void MeshSystem::Tick()
{
    System::Tick();
}

MeshHandle MeshSystem::UploadRenderItem(const RenderItem& inRenderItem)
{
    return myRenderItems.Add(inRenderItem);
}

void MeshSystem::RemoveRenderItem(const MeshHandle inHandle)
{
    myRenderItems.RemoveIndex(inHandle);
}

const VulkanDynamicBuffer<RenderItem>& MeshSystem::GetBuffer() const
{
    return myRenderItems;
}