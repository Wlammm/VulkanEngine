#include "EnginePch.h"
#include "StaticMeshComponent.h"

#include "Engine.h"
#include "TransformComponent.h"
#include "Assets/Model.h"
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

void StaticMeshComponent::RegisterMeshesToObjectSystem() const
{
    if(!myModel)
        return;
    
    for(const Mesh* mesh : myModel->GetMeshes())
    {
        Engine::GetEngineSystem<ObjectSystem>().AddObject(GetTransform().GetMatrix(), mesh);
    }
}