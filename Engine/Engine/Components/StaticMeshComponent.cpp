#include "EnginePch.h"
#include "StaticMeshComponent.h"

#include "Engine.h"
#include "TransformComponent.h"
#include "AssetRegistry/AssetRegistry.h"
#include "Assets/Material.h"
#include "Assets/Model.h"
#include "Assets/Texture.h"
#include "Rendering/Mesh.h"
#include "Shaders/MeshStructs.hpp"
#include "Vulkan/GPUSceneSystem.h"

void StaticMeshComponent::OnCreate()
{
    Component::OnCreate();
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
    MarkRenderStateDirty();
}

Model* StaticMeshComponent::GetModel() const
{
    return myModel;
}

void StaticMeshComponent::SetMaterial(Material* inMaterial, const uint inIndex)
{
    myMaterials[inIndex] = inMaterial;
    MarkRenderStateDirty();
}

Material* StaticMeshComponent::GetMaterial(const uint inIndex) const
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

Material* StaticMeshComponent::GetMaterialForMesh(Mesh* inMesh) const
{
    uint index = myModel->GetMeshes().FindIndex(inMesh);
    check(index != -1 && "Mesh is not part of the model.");
    return GetMaterial(index);
}

void StaticMeshComponent::OnRenderStateDirty()
{
    RemoveFromGPUScene();
    
    if(!myModel)
        return;

    myMaterials.Resize(myModel->GetMeshes().size());
    for(int i = 0; i < myModel->GetMeshes().size(); ++i)
    {
        const Mesh* mesh = myModel->GetMeshes()[i];

        std::filesystem::path albedoPath = AssetRegistry::GetPathFromAssetName(mesh->GetAlbedoPath());
        std::filesystem::path normalPath = AssetRegistry::GetPathFromAssetName(mesh->GetNormalPath());
        std::filesystem::path materialPath = AssetRegistry::GetPathFromAssetName(mesh->GetMaterialPath());
        
        if(std::filesystem::exists(albedoPath) && std::filesystem::exists(normalPath) && std::filesystem::exists(materialPath))
        {
            if(myMaterials[i])
                del(myMaterials[i]);
            
            myMaterials[i] = new Material(albedoPath, normalPath, materialPath);
        }

        MeshInstanceData data{ GetTransform()->GetMatrix(), mesh->GetHandle() };

        if(myMaterials[i])
        {
            data.myAlbedoIndex = myMaterials[i]->GetAlbedo()->GetBindlessIndex();
            data.myNormalIndex = myMaterials[i]->GetNormal()->GetBindlessIndex();
            data.myMaterialIndex = myMaterials[i]->GetMaterial()->GetBindlessIndex();
        }
        
        myMeshInstances.Add(Engine::GetEngineSystem<GPUSceneSystem>().AddMeshInstance(data));
    }
}

void StaticMeshComponent::RemoveFromGPUScene()
{
    for(const MeshInstanceIndex instanceIndex : myMeshInstances)
    {
        Engine::GetEngineSystem<GPUSceneSystem>().RemoveMeshInstance(instanceIndex);
    }
    myMeshInstances.Clear();
}
