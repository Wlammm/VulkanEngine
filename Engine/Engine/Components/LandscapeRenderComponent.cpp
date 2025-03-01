#include "EnginePch.h"
#include "LandscapeRenderComponent.h"

#include "Engine.h"
#include "TransformComponent.h"
#include <PerlinNoise.hpp>

#include "AssetRegistry/AssetRegistry.h"
#include "Assets/Material.h"
#include "Assets/Texture.h"
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
#include "World/World.h"

LandscapeRenderComponent::LandscapeRenderComponent()
{
    
}

LandscapeRenderComponent::~LandscapeRenderComponent()
{
    // Remove buffers.
}

void LandscapeRenderComponent::OnCreate()
{
    GetWorld()->GetAssetRegistry().GetAssetAsync<Material>("Assets/Materials/LandscapeGrass/LandscapeGrass.mat", [this](Material* inMaterial)
    {
        myMaterial = inMaterial;
        MarkRenderStateDirty();
    });
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

    if(myMaterial)
    {
        data.myAlbedoIndex = myMaterial->GetAlbedo()->GetBindlessIndex();
        data.myNormalIndex = myMaterial->GetNormal()->GetBindlessIndex();
        data.myMaterialIndex = myMaterial->GetMaterial()->GetBindlessIndex();
    }
    
    myMeshInstance = Engine::GetEngineSystem<GPUSceneSystem>().AddMeshInstance(data);
}

glm::vec3 ComputeTangent(const glm::vec3& edge1, const glm::vec3& edge2,
                         const glm::vec2& uv1, const glm::vec2& uv2, const glm::vec2& uv3)
{
    glm::vec2 deltaUV1 = uv2 - uv1;
    glm::vec2 deltaUV2 = uv3 - uv1;

    float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    glm::vec3 tangent;
    tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

    return glm::normalize(tangent);
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
            vertex.myTexCoords[0] = {xPos / 100.0f, zPos / 100.0f };
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

            glm::vec2 uv1 = vertices[topLeft].myTexCoords[0];
            glm::vec2 uv2 = vertices[bottomLeft].myTexCoords[0];
            glm::vec2 uv3 = vertices[topRight].myTexCoords[0];

            glm::vec3 tangent1 = ComputeTangent(edge1, edge2, uv1, uv2, uv3);

            
            vertices[bottomLeft].myNormal += normal1;
            vertices[topLeft].myNormal += normal1;
            vertices[bottomRight].myNormal += normal1;

            indices.Add(topRight);
            indices.Add(bottomLeft);
            indices.Add(bottomRight);

            edge1 = vertices[bottomLeft].myPosition - vertices[topRight].myPosition;
            edge2 = vertices[topRight].myPosition - vertices[bottomRight].myPosition;
            glm::vec3 normal2 = glm::normalize(glm::cross(edge2, edge1));
            

            uv1 = vertices[topRight].myTexCoords[0];
            uv2 = vertices[bottomLeft].myTexCoords[0];
            uv3 = vertices[bottomRight].myTexCoords[0];

            glm::vec3 tangent2 = ComputeTangent(edge1, edge2, uv1, uv2, uv3);
            
            vertices[topRight].myNormal += normal2;
            vertices[bottomRight].myNormal += normal2;
            vertices[topLeft].myNormal += normal2;

            vertices[topRight].myTangents += tangent2;
            vertices[bottomRight].myTangents += tangent2;
            vertices[bottomLeft].myTangents += tangent2;
        }
    }
    
    for(Vertex& vertex : vertices)
    {
        vertex.myNormal = glm::normalize(vertex.myNormal);
        vertex.myTangents = glm::normalize(vertex.myTangents);
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
}
