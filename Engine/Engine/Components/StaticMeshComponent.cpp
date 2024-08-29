#include "EnginePch.h"
#include "StaticMeshComponent.h"

#include "Engine.h"
#include "TransformComponent.h"
#include "AssetRegistry/AssetRegistry.h"
#include "Assets/Material.h"
#include "Assets/Model.h"
#include "Rendering/Mesh.h"
#include "Vulkan/ObjectSystem.h"

void StaticMeshComponent::Start()
{
    Component::Start();
}

StaticMeshComponent::~StaticMeshComponent()
{
    if(!myModel)
        return;

    // Remove object from here.    
}

void StaticMeshComponent::SetModel(Model* inModel)
{
    myModel = inModel;
    RegisterMeshesToObjectSystem();
}

Model* StaticMeshComponent::GetModel() const
{
    return myModel;
}

void StaticMeshComponent::SetMaterial(Material* inMaterial, const uint inIndex)
{
    myMaterials[inIndex] = inMaterial;    
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

void StaticMeshComponent::RegisterMeshesToObjectSystem()
{
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
            myMaterials[i] = new Material(albedoPath, normalPath, materialPath);
        }
        
        Engine::GetEngineSystem<ObjectSystem>().AddObject(GetTransform().GetMatrix(), mesh, myMaterials[i]);
    }
}