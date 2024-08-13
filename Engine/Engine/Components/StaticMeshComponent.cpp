#include "EnginePch.h"
#include "StaticMeshComponent.h"

#include "Engine.h"
#include "Assets/Model.h"
#include "Vulkan/ObjectSystem.h"

void StaticMeshComponent::Start()
{
    Component::Start();

    if(!myModel)
        return;
    
    for(const MeshHandle meshHandle : myModel->GetMeshHandles())
    {
        Engine::GetEngineSystem<ObjectSystem>().AddObject(glm::mat4(), meshHandle);
    }
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
}

Model* StaticMeshComponent::GetModel() const
{
    return myModel;
}