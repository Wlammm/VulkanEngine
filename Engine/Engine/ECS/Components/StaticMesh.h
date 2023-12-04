#pragma once
#include "Engine/Assets/Model.h"

struct StaticMesh
{
	StaticMesh() = default;
	StaticMesh(const std::filesystem::path& inPath);

	class Model* myModel = nullptr;
	List<class Material*> myMaterials{};
};
