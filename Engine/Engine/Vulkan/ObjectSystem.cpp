#include "EnginePch.h"
#include "ObjectSystem.h"

#include "Engine.h"
#include "ResizableBuffer.h"
#include "Components/StaticMeshComponent.h"
#include "ComponentSystem/ComponentSystem.h"
#include "World/World.h"

#include "Shaders/MeshStructs.hpp"

ObjectSystem::ObjectSystem()
{
    myBuffer = new ResizableBuffer(VulkanAllocator::AllocateBuffer_TS(
        "ObjectBuffer",
        vk::BufferCreateInfo()
                .setUsage(vk::BufferUsageFlagBits::eStorageBuffer | vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eTransferSrc)
                .setSize(sizeof(ObjectData) * 4),
        VMA_MEMORY_USAGE_AUTO));
}

ObjectSystem::~ObjectSystem()
{
    del(myBuffer);
}

void ObjectSystem::Tick()
{
    UploadQueuedObjects();
    
    //DO_ONCE(
    //    for(auto [entity, mesh, transform] : Engine::GetWorld().GetRegistry().IterateComponents<const StaticMesh, const Transform>())
    //    {
    //        if(!mesh.myModel)
    //            continue;
    //        
    //        for(const MeshHandle& meshHandle : mesh.myModel->GetMeshHandles())
    //        {
    //            AddObject(transform.GetMatrix(), meshHandle);
    //        }
    //    }
    //);


    
    //static bool firstTime = true;
    //if(!firstTime)
    //    return;
    //firstTime = false;
//
    //auto view = Engine::GetWorld().GetRegistry().view<const StaticMesh, const Transform>();
    //for(auto [ent, staticMesh, transform] : view.each())
    //{
    //    if(!staticMesh.myModel)
    //        continue;
//
    //    for(const MeshHandle& handle : staticMesh.myModel->GetMeshHandles())
    //    {
    //        AddObjectToBuffer(transform.GetMatrix(), handle);
    //    }
    //}
}

const ResizableBuffer* ObjectSystem::GetBuffer() const
{
    return myBuffer;
}

uint ObjectSystem::GetNumObjects() const
{
    return myNumObjects;
}

void ObjectSystem::UploadQueuedObjects()
{
    if(myQueuedObjects.IsEmpty())
        return;

    if(myBuffer->GetBuffer()->GetSize() < sizeof(ObjectData) * myNumObjects)
    {
        myBuffer->Resize(MathUtils::UpperPowerOfTwo(sizeof(ObjectData) * myNumObjects));
    }

    myBuffer->GetBuffer()->SetData(myQueuedObjects.data(), sizeof(ObjectData) * myQueuedObjects.size(), sizeof(ObjectData) * myNumUploadedObjects);
    myNumUploadedObjects += myQueuedObjects.size();
    myQueuedObjects.Clear();
}

void ObjectSystem::AddObject(const glm::mat4& inTransform, const MeshHandle inMesh)
{
    myQueuedObjects.Add({inTransform, inMesh});
    myNumObjects++;
    return;
    
    myNumObjects++;

    if(sizeof(ObjectData) * myNumObjects > myBuffer->GetBuffer()->GetSize())
    {
        myBuffer->Resize(MathUtils::UpperPowerOfTwo(sizeof(ObjectData) * myNumObjects));
    }

    ObjectData data{inTransform, inMesh};
    myBuffer->GetBuffer()->SetData(&data, sizeof(ObjectData), sizeof(ObjectData) * (myNumObjects - 1));
}