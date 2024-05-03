#include "EnginePch.h"
#include "ObjectSystem.h"

#include "Engine.h"
#include "ECS/Components/StaticMesh.h"
#include "ECS/Components/Transform.h"
#include "World/World.h"

ObjectSystem::ObjectSystem()
{
    
}

void ObjectSystem::Tick()
{
    System::Tick();

    static bool firstTime = true;

    if(!firstTime)
        return;

    firstTime = false;
    
    auto view = Engine::GetWorld().GetRegistry().view<const StaticMesh, const Transform>();
    for(auto [ent, staticMesh, transform] : view.each())
    {
        if(!staticMesh.myModel)
            continue;
        
        for(const MeshHandle& handle : staticMesh.myModel->GetMeshHandles())
        {
            myBuffer.Add(RenderObject(transform.GetMatrix(), handle));
        }
    }
}
const VulkanDynamicBuffer<RenderObject>& ObjectSystem::GetBuffer() const
{
    return myBuffer;
}