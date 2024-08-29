#include "EnginePch.h"
#include "ObjectSystem.h"

#include "Engine.h"
#include "ResizableBuffer.h"
#include "VulkanAllocator.h"
#include "Assets/Material.h"
#include "Assets/Texture.h"
#include "Components/StaticMeshComponent.h"
#include "ComponentSystem/ComponentSystem.h"
#include "Rendering/Mesh.h"
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
    VulkanAllocator::DestroyBuffer_TS(myBuffer);
}

const ResizableBuffer* ObjectSystem::GetBuffer() const
{
    return myBuffer;
}

uint ObjectSystem::GetNumObjects() const
{
    return myNumObjects;
}

void ObjectSystem::AddObject(const glm::mat4& inTransform, const Mesh* inMesh, Material* inMaterial)
{
    ObjectData data{inTransform, inMesh->GetHandle() };

    if(inMaterial)
    {
        data.myAlbedoIndex = inMaterial->GetAlbedo()->GetBindlessIndex();
        data.myNormalIndex = inMaterial->GetNormal()->GetBindlessIndex();
        data.myMaterialIndex = inMaterial->GetMaterial()->GetBindlessIndex();
    }
    else
    {
        data.myAlbedoIndex = -1;
        data.myNormalIndex = -1;
        data.myMaterialIndex = -1;
    }
    
    // Offset by the first num objects.
    myBuffer->SetData(&data, sizeof(ObjectData), 32 + sizeof(ObjectData) * myNumObjects);
    myNumObjects++;

    // Always update the object count.
    myBuffer->SetData(&myNumObjects, sizeof(uint), 0);
}