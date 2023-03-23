#pragma once

#include "Rendering/Mesh.h"

class Model
{
public:
	struct CreateInfo
	{
		bool InvertY = false;
		bool FlipIndicies = false;
	};

public:
	bool IsValid() const;

	const List<Mesh>& GetMeshes() const;

private:
	// ------- TEMP -------
	friend class RenderSystem;

	// Only asset registry should create instances of this class.
	friend class AssetRegistry;
	Model();
	Model(const std::filesystem::path& inPath, const CreateInfo& inCreateInfo);
	~Model();


private:
	bool myIsValid = false;
	std::filesystem::path myPath = "";
	List<Mesh> myMeshes{};
};