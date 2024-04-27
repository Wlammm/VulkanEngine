#pragma once

#include "Engine/Rendering/Mesh.hpp"

class Model
{
public:
	bool IsValid() const;
	const List<Mesh>& GetMeshes() const;

private:
	// Only get models via an asset registry.
	friend class ModelFactory;
	Model(const List<Mesh>& inMeshes);

	friend class AssetRegistry;
	~Model();

private:
	bool myIsValid = false;
	std::filesystem::path myPath = "";
	List<Mesh> myMeshes{};
};