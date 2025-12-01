#include "EnginePch.h"
#include "StaticMeshComponent.h"

#include "Engine/Engine.h"
#include "TransformComponent.h"
#include "Engine/AssetRegistry/AssetRegistry.h"
#include "Engine/AssetRegistry/AssetUtils.h"
#include "Engine/Assets/Material.h"
#include "Engine/Assets/Model.h"
#include "Engine/Assets/Texture.h"
#include "Engine/Rendering/Mesh.h"
#include "Engine/Rendering/SharedWithShaders/MeshStructs.hpp"
#include "Engine/Vulkan/GPUSceneSystem.h"

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
        SetModel(Engine::GetEngineSystem<AssetRegistry>().GetAsset<Model>(myPath));
    }
}

StaticMeshComponent::~StaticMeshComponent()
{
    if(!myModel)
        return;
    
    RemoveFromGPUScene(); 
}

void StaticMeshComponent::SetModel(SharedPtr<Model> inModel)
{
    if(myModel == inModel)
        return;

    myModel = inModel;

    UpdateMaterialsForNewMesh();
    
    MarkRenderStateDirty();
}

SharedPtr<Model> StaticMeshComponent::GetModel() const
{
    return myModel;
}

void StaticMeshComponent::SetMaterialAsync(const std::filesystem::path& inMaterialPath, const uint inIndex)
{
    LOG_WARNING("StaticMeshComponent::SetMaterialAsync isnt async right now. FIXX");
    SetMaterial(inMaterialPath, inIndex);
    //GetWorld()->GetAssetRegistry().GetAssetAsync<Material>(inMaterialPath, [this, inIndex](Material* inMaterial)
    //{
    //    SetMaterial(inMaterial, inIndex);
    //});
}

void StaticMeshComponent::SetMaterial(SharedPtr<Material> inMaterial, const uint inIndex)
{
    // TODO: This might in some rare occasions happen before the mesh has loaded. We should probably add checks for that.
    myMaterials[inIndex] = inMaterial;
    MarkRenderStateDirty();
}

void StaticMeshComponent::SetMaterial(const std::filesystem::path& inMaterialPath, const uint inIndex)
{
    SetMaterial(Engine::GetEngineSystem<AssetRegistry>().GetAsset<Material>(inMaterialPath), inIndex);
}

SharedPtr<Material> StaticMeshComponent::GetMaterial(const uint inIndex) const
{
    if(!myMaterials.IsValidIndex(inIndex))
        return nullptr;
    
    return myMaterials[inIndex];
}

void StaticMeshComponent::SetMaterialForMesh(SharedPtr<Material> inMaterial, Mesh* inMesh)
{
    uint index = myModel->GetMeshes().FindIndex(inMesh);
    check(index != -1 && "Mesh is not part of the model.");
    SetMaterial(inMaterial, index);
}

SharedPtr<Material> StaticMeshComponent::GetMaterialForMesh(Mesh* inMesh) const
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
    for(int i = myModel->GetMeshes().size(); i < myMeshInstances.size(); i++)
    {
        Engine::GetEngineSystem<GPUSceneSystem>().RemoveMeshInstance(myMeshInstances[i]);
    }
    
    if (myMeshInstances.size() > myModel->GetMeshes().size())
        myMeshInstances.Resize(myModel->GetMeshes().size());
    
    for(int meshIndex = 0; meshIndex < myModel->GetMeshes().size(); ++meshIndex)
    {
        const Mesh* mesh = myModel->GetMeshes()[meshIndex];
        
        MeshInstanceData data{ .myToWorld=GetTransform().GetMatrix(), .myMeshIndex=mesh->GetHandle(), .myDepthWriteEnabled=myShouldWriteDepth };

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

void StaticMeshComponent::UpdateMaterialsForNewMesh()
{
    // Update materials for the new mesh.
    myMaterials.Resize(myModel->GetMeshes().size());
    for(int meshIndex = 0; meshIndex < myModel->GetMeshes().size(); meshIndex++)
    {
        const Mesh* mesh = myModel->GetMeshes()[meshIndex];
        std::filesystem::path albedoPath = AssetUtils::GetSourcePathFromAssetName(mesh->GetAlbedoPath());
        std::filesystem::path normalPath = AssetUtils::GetSourcePathFromAssetName(mesh->GetNormalPath());
        std::filesystem::path materialPath = AssetUtils::GetSourcePathFromAssetName(mesh->GetMaterialPath());

        if(std::filesystem::exists(albedoPath) && std::filesystem::exists(normalPath) && std::filesystem::exists(materialPath))
        {
            const std::string generatedString = GENERATED_MATERIAL_PREFIX + albedoPath.string() + normalPath.string() + materialPath.string();

            if(SharedPtr<Material> material = Engine::GetEngineSystem<AssetRegistry>().GetAsset<Material>(generatedString))
            {
                myMaterials[meshIndex] = material;
                MarkRenderStateDirty();
            }
            else
            {
                SharedPtr<Material> newMaterial = Engine::GetEngineSystem<AssetRegistry>().CreateNewAsset<Material>(generatedString);
                newMaterial->SetAlbedo(albedoPath);
                newMaterial->SetNormal(normalPath);
                newMaterial->SetMaterial(materialPath);
                myMaterials[meshIndex] = newMaterial;
                MarkRenderStateDirty();
            }
        }
        else
        {
            // TODO: This should be default material.
            myMaterials[meshIndex] = nullptr;
        }
    }
}

void StaticMeshComponent::OnModelChangedFromInspector()
{
    MarkRenderStateDirty();
    UpdateMaterialsForNewMesh();
}
