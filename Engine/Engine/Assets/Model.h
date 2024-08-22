#pragma once

#include "AssetRegistry/Asset.h"
#include "Rendering/Vertex.hpp"

class Mesh;
using CachePath = std::filesystem::path;

struct SerializationMeshData
{
	List<Vertex> myVertices{};
	List<uint> myIndices{};
	glm::vec4 mySphereCenterBounds{};
};

class Model : public Asset
{
	static constexpr uint BinaryVersion = 1;
	
public:
	Coroutine<void, void, false> Load(const std::filesystem::path& inPath) override;
	void Unload() override;

	const List<Mesh*>& GetMeshes() const;
	
private:
	static CachePath GetCachedFilePath(const std::filesystem::path& inPath);
	static bool IsCached(const std::filesystem::path& inPath);
	
	bool TryLoadMeshDatasFromCache(const CachePath& inPath, List<SerializationMeshData>& outMeshDatas);
	List<SerializationMeshData> LoadMeshDatasFromFbx(const std::filesystem::path& inPath);
	void SaveToCache(const List<SerializationMeshData>& inMeshDatas, const std::filesystem::path& inSourcePath);

	void InitializeFromMeshData(List<SerializationMeshData> inMeshDatas);
	
	static glm::vec4 CalculateSphereBounds(const List<Vertex>& inVertices);

private:
	List<Mesh*> myMeshes{};
};