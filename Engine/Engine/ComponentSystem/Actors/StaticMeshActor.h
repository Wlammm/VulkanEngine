#pragma once
#include "Engine/Components/StaticMeshComponent.h"
#include "Engine/ComponentSystem/Actor.h"

class StaticMeshActor : public Actor
{
public:
    StaticMeshActor() = default;
    StaticMeshActor(const std::filesystem::path& inPath);
    StaticMeshActor(SharedPtr<Model> inModel);
    
    StaticMeshComponent& GetStaticMeshComponent();

private:
    META(SerializeField)
    StaticMeshComponent myStaticMeshComponent;
};
