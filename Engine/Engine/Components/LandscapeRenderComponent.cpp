#include "EnginePch.h"
#include "LandscapeRenderComponent.h"

#include "Engine.h"
#include "TransformComponent.h"
#include <PerlinNoise.hpp>
#include "Rendering/IndexBufferSystem.h"
#include "Rendering/Mesh.h"
#include "Rendering/MeshSystem.h"
#include "Rendering/MeshUtils.h"
#include "Rendering/VertexBufferSystem.h"
#include "Vulkan/GPUSceneSystem.h"
#include "Vulkan/VulkanAllocator.h"
#include "Vulkan/VulkanBuffer.h"
#include "Vulkan/VulkanContext.h"
#include "Vulkan/VulkanSwapChain.h"

LandscapeRenderComponent::LandscapeRenderComponent()
{
    
}

LandscapeRenderComponent::~LandscapeRenderComponent()
{
    // Remove buffers.
}

void LandscapeRenderComponent::OnCreate()
{
    CreateLandscapeMesh();
}

void LandscapeRenderComponent::Tick()
{
    
}

void LandscapeRenderComponent::OnRenderStateDirty()
{
    if(!myMesh)
        return;
    
    LOG("Adding Landscape on frame: %i", VulkanContext::GetSwapChain().GetFrameIndex());
    if(myMeshInstance != (uint)-1)
    {
        Engine::GetEngineSystem<GPUSceneSystem>().RemoveMeshInstance(myMeshInstance);
    }
    
    MeshInstanceData data{ GetTransform()->GetMatrix(), myMesh->GetHandle() }; 
    myMeshInstance = Engine::GetEngineSystem<GPUSceneSystem>().AddMeshInstance(data);
}

void LandscapeRenderComponent::CreateLandscapeMesh()
{
    constexpr int chunkSize = 64;
    constexpr float chunkScale = 20000.0f;

    const siv::PerlinNoise::seed_type seed = 321u;
    const siv::BasicPerlinNoise<float> perlinNoise{ seed };
    
    List<Vertex> vertices;
    List<uint> indices;

    for (int z = 0; z < chunkSize; ++z)
    {
        for (int x = 0; x < chunkSize; ++x)
        {
            float xPos = (static_cast<float>(x) / static_cast<float>(chunkSize - 1) - 0.5f) * chunkScale;
            float zPos = (static_cast<float>(z) / static_cast<float>(chunkSize - 1) - 0.5f) * chunkScale;
            Vertex vertex{};

            constexpr float frequency = 0.0001f;
            constexpr float amplitude = 3000.0f;
            float yPos = perlinNoise.octave2D_01((static_cast<float>(xPos) * frequency), (static_cast<float>(zPos) * frequency), 4) * amplitude;
            
            vertex.myPosition = { xPos, yPos, zPos };
            vertices.Add(vertex);
        }
    }

    // Insert indices and accumulate normals.
    for (int z = 0; z < chunkSize - 1; ++z)
    {
        for (int x = 0; x < chunkSize - 1; ++x)
        {
            uint topLeft = z * chunkSize + x;
            uint topRight = topLeft + 1;
            uint bottomLeft = (z + 1) * chunkSize + x;
            uint bottomRight = bottomLeft + 1;

            indices.Add(topLeft);
            indices.Add(bottomLeft);
            indices.Add(topRight);

            glm::vec3 edge1 = vertices[bottomLeft].myPosition - vertices[topLeft].myPosition;
            glm::vec3 edge2 = vertices[topLeft].myPosition - vertices[topRight].myPosition;
            glm::vec3 normal1 = glm::normalize(glm::cross(edge2, edge1));

            vertices[bottomLeft].myNormal += normal1;
            vertices[topLeft].myNormal += normal1;
            vertices[bottomRight].myNormal += normal1;

            indices.Add(topRight);
            indices.Add(bottomLeft);
            indices.Add(bottomRight);

            edge1 = vertices[bottomLeft].myPosition - vertices[topRight].myPosition;
            edge2 = vertices[topRight].myPosition - vertices[bottomRight].myPosition;
            glm::vec3 normal2 = glm::normalize(glm::cross(edge2, edge1));

            vertices[topRight].myNormal += normal2;
            vertices[bottomRight].myNormal += normal2;
            vertices[topLeft].myNormal += normal2;
        }
    }
    
    for(Vertex& vertex : vertices)
    {
        vertex.myNormal = glm::normalize(vertex.myNormal);
    }

    // Upload staging buffers.
    const uint vertexSize = vertices.size() * sizeof(Vertex);
    VulkanBuffer* stagingVertexBuffer = VulkanAllocator::AllocateBuffer_TS("VulkanBuffer-Staging", VulkanBuffer::StagingCreateInfo(vertexSize), VMA_MEMORY_USAGE_AUTO, true);
    stagingVertexBuffer->SetData(vertices.data(), vertexSize);

    const uint indexSize = indices.size() * sizeof(uint);
    VulkanBuffer* stagingIndexBuffer = VulkanAllocator::AllocateBuffer_TS("VulkanBuffer-Staging", VulkanBuffer::StagingCreateInfo(indexSize), VMA_MEMORY_USAGE_AUTO, true);
    stagingIndexBuffer->SetData(indices.data(), indexSize);

    // Create buffers from staged data.
    myVertexBuffer = Engine::GetEngineSystem<VertexBufferSystem>().UploadVertexBuffer(stagingVertexBuffer, vertices.size());
    VulkanAllocator::DestroyBuffer_TS(stagingVertexBuffer);
		
    myIndexBuffer = Engine::GetEngineSystem<IndexBufferSystem>().UploadIndexBuffer(stagingIndexBuffer, indices.size());
    VulkanAllocator::DestroyBuffer_TS(stagingIndexBuffer);

    const glm::vec4 sphereBounds = MeshUtils::CalculateSphereBounds(vertices);

    myMesh = Engine::GetEngineSystem<MeshSystem>().UploadMesh(myVertexBuffer, myIndexBuffer, sphereBounds);
    MarkRenderStateDirty();
    
    //MeshInstanceData data{ GetTransform()->GetMatrix(), myMesh->GetHandle() };
    //myMeshInstance = Engine::GetEngineSystem<GPUSceneSystem>().AddMeshInstance(data);
}
