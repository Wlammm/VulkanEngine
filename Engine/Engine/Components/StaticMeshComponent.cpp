#include "EnginePch.h"
#include "StaticMeshComponent.h"

#include "Engine/Engine.h"
#include "TransformComponent.h"
#include "Engine/AssetRegistry/AssetRegistry.h"
#include "Engine/Assets/Material.h"
#include "Engine/Assets/Model.h"
#include "Engine/Assets/Texture.h"
#include "Engine/Rendering/Mesh.h"
#include "Engine/Shaders/MeshStructs.hpp"
#include "Engine/Vulkan/GPUSceneSystem.h"
#include "Engine/World/World.h"

StaticMeshComponent::StaticMeshComponent()
{
}

StaticMeshComponent::StaticMeshComponent(const std::filesystem::path& inPath)
{
    myPath = inPath;
}

void StaticMeshComponent::OnCreate()
{
    Component::OnCreate();

    if (!myPath.empty())
    {
        GetWorld()->GetAssetRegistry().GetAssetAsync<Model>(myPath, [this](Model* inModel)
        {
            SetModel(inModel);
        });
    }
}

StaticMeshComponent::~StaticMeshComponent()
{
    if(!myModel)
        return;
    
    RemoveFromGPUScene(); 
}

void StaticMeshComponent::SetModel(Model* inModel)
{
    if(myModel == inModel)
        return;

    myModel = inModel;

    // Update materials for the new mesh.
    myMaterials.Resize(myModel->GetMeshes().size());
    for(int meshIndex = 0; meshIndex < myModel->GetMeshes().size(); meshIndex++)
    {
        const Mesh* mesh = myModel->GetMeshes()[meshIndex];
        std::filesystem::path albedoPath = AssetRegistry::GetPathFromAssetName(mesh->GetAlbedoPath());
        std::filesystem::path normalPath = AssetRegistry::GetPathFromAssetName(mesh->GetNormalPath());
        std::filesystem::path materialPath = AssetRegistry::GetPathFromAssetName(mesh->GetMaterialPath());

        if(std::filesystem::exists(albedoPath) && std::filesystem::exists(normalPath) && std::filesystem::exists(materialPath))
        {
            const std::string generatedString = GENERATED_MATERIAL_PREFIX + albedoPath.string() + normalPath.string() + materialPath.string();

            if(GetWorld()->GetAssetRegistry().HasAsset<Material>(generatedString))
            {
                GetWorld()->GetAssetRegistry().GetAssetAsync<Material>(generatedString, [this, meshIndex](Material* inMaterial)
                {
                    myMaterials[meshIndex] = inMaterial;
                    MarkRenderStateDirty();
                });                
            }
            else
            {
                GetWorld()->GetAssetRegistry().GetAssetAsync<Material>(generatedString, [this, meshIndex](Material* inMaterial)
                {
                    myMaterials[meshIndex] = inMaterial;
                    MarkRenderStateDirty();
                }, albedoPath, normalPath, materialPath);
            }
        }
        else
        {
            // TODO: This should be default material.
            myMaterials[meshIndex] = nullptr;
        }
    }
    
    MarkRenderStateDirty();
}

Model* StaticMeshComponent::GetModel() const
{
    return myModel;
}

void StaticMeshComponent::SetMaterialAsync(const std::filesystem::path& inMaterialPath, const uint inIndex)
{
    GetWorld()->GetAssetRegistry().GetAssetAsync<Material>(inMaterialPath, [this, inIndex](Material* inMaterial)
    {
        SetMaterial(inMaterial, inIndex);
    });
}

void StaticMeshComponent::SetMaterial(Material* inMaterial, const uint inIndex)
{
    myMaterials[inIndex] = inMaterial;
    MarkRenderStateDirty();
}

const Material* StaticMeshComponent::GetMaterial(const uint inIndex) const
{
    if(!myMaterials.IsValidIndex(inIndex))
        return nullptr;
    
    return myMaterials[inIndex];
}

void StaticMeshComponent::SetMaterialForMesh(Material* inMaterial, Mesh* inMesh)
{
    uint index = myModel->GetMeshes().FindIndex(inMesh);
    check(index != -1 && "Mesh is not part of the model.");
    SetMaterial(inMaterial, index);
}

const Material* StaticMeshComponent::GetMaterialForMesh(Mesh* inMesh) const
{
    uint index = myModel->GetMeshes().FindIndex(inMesh);
    check(index != -1 && "Mesh is not part of the model.");
    return GetMaterial(index);
}

void StaticMeshComponent::OnRenderStateDirty()
{
    ZoneScoped;
    if(!myModel)
    {
        // We need to remove all active mesh instances on the gpu.
        if(!myMeshInstances.IsEmpty())
            RemoveFromGPUScene();
        
        return;
    }

    // Remove all mesh instances that are more than the mesh count.
    for(int instanceIndex = myModel->GetMeshes().size(); instanceIndex < myMeshInstances.size(); instanceIndex++)
    {
        Engine::GetEngineSystem<GPUSceneSystem>().RemoveMeshInstance(instanceIndex);
    }
    
    for(int meshIndex = 0; meshIndex < myModel->GetMeshes().size(); ++meshIndex)
    {
        const Mesh* mesh = myModel->GetMeshes()[meshIndex];
        
        MeshInstanceData data{ .myToWorld=GetTransform()->GetMatrix(), .myMeshIndex=mesh->GetHandle(), .myDepthWriteEnabled=myShouldWriteDepth };

        check(myMaterials.IsValidIndex(meshIndex));
        if(myMaterials[meshIndex] && myMaterials[meshIndex]->IsValid())
        {
            data.myAlbedoIndex = myMaterials[meshIndex]->GetAlbedo()->GetBindlessIndex();
            data.myNormalIndex = myMaterials[meshIndex]->GetNormal()->GetBindlessIndex();
            data.myMaterialIndex = myMaterials[meshIndex]->GetMaterial()->GetBindlessIndex();
        }
        
        // If we already have a mesh instance we can just update that instead of adding a new one which is cheaper
        if(myMeshInstances.IsValidIndex(meshIndex))
        {
            Engine::GetEngineSystem<GPUSceneSystem>().UpdateMeshInstance(myMeshInstances[meshIndex], data);
            continue;
        }

        // Otherwise we add a new mesh instance for this one.
        myMeshInstances.Add(Engine::GetEngineSystem<GPUSceneSystem>().AddMeshInstance(data));
    }
}

void StaticMeshComponent::SetDepthWriteEnabled(const bool inEnabled)
{
    if (myShouldWriteDepth == inEnabled)
        return;
    
    myShouldWriteDepth = inEnabled;
    MarkRenderStateDirty();
}

void StaticMeshComponent::RemoveFromGPUScene()
{
    for(const MeshInstanceIndex instanceIndex : myMeshInstances)
    {
        Engine::GetEngineSystem<GPUSceneSystem>().RemoveMeshInstance(instanceIndex);
    }
    myMeshInstances.Clear();
}
