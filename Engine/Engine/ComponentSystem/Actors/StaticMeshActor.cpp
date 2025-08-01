#include "EnginePch.h"
#include "StaticMeshActor.h"

StaticMeshActor::StaticMeshActor(const std::filesystem::path& inPath)
    : myStaticMeshComponent(inPath)
{
}

StaticMeshActor::StaticMeshActor(Model* inModel)
{
    myStaticMeshComponent.SetModel(inModel);
}

StaticMeshComponent& StaticMeshActor::GetStaticMeshComponent()
{
    return myStaticMeshComponent;
}
