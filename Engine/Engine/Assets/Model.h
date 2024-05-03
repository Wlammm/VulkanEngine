#pragma once

#include "Rendering/Mesh.hpp"
#include "Rendering/MeshSystem.h"

class Model
{
public:
	bool IsValid() const;
	const List<Mesh>& GetMeshes() const;
	const List<MeshHandle>& GetMeshHandles() const;

private:
	// Only get models via an asset registry.
	friend class ModelFactory;
	Model(const List<Mesh>& inMeshes, const List<MeshHandle>& inHandles);

	friend class AssetRegistry;
	~Model();

private:
	bool myIsValid = false;
	std::filesystem::path myPath = "";
	List<Mesh> myMeshes{};
	List<MeshHandle> myMeshHandles{};
};