#include "EnginePch.h"
#include "StaticMesh.h"
#include "Engine.h"
#include "World/World.h"
#include "Assets/AssetRegistry.h"

StaticMesh::StaticMesh(const std::filesystem::path& inPath, const Model::CreateInfo& inCreateInfo)
{
	myModel = Engine::GetWorld().GetAssetRegistry().GetModel(inPath, inCreateInfo);
}