#pragma once
#include "Engine/Assets/Model.h"

struct StaticMesh
{
	StaticMesh() = default;
	StaticMesh(const std::filesystem::path& inPath, const Model::CreateInfo& inCreateInfo = Model::CreateInfo());

	class Model* myModel = nullptr;
	List<class Material*> myMaterials{};
};
