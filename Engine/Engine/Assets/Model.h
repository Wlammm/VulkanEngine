#pragma once

#include "AssetRegistry/Asset.h"
#include "Rendering/Vertex.hpp"

class VulkanBuffer;
class Mesh;
using CachePath = std::filesystem::path;

struct SerializationMeshData
{
	List<Vertex> myVertices{};
	List<uint> myIndices{};
	glm::vec4 mySphereCenterBounds{};
	VulkanBuffer* myStagingVertexBuffer = nullptr;
	VulkanBuffer* myStagingIndexBuffer = nullptr;
	std::string myAlbedoPath;
	std::string myNormalPath;
	std::string myMaterialPath;
};

class Model : public Asset
{
	static constexpr uint BinaryVersion = 1;
	
public:
	Coroutine<void, void, false> Load(const std::filesystem::path inPath) override;
	void Unload() override;

	const List<Mesh*>& GetMeshes() const;

	
private:
	static CachePath GetCachedFilePath(const std::filesystem::path& inPath);
	static bool IsCached(const std::filesystem::path& inPath);
	
	bool TryLoadMeshDatasFromCache(const CachePath& inPath, List<SerializationMeshData>& outMeshDatas);
	List<SerializationMeshData> LoadMeshDatasFromFbx(const std::filesystem::path& inPath);
	void SaveToCache(const List<SerializationMeshData>& inMeshDatas, const std::filesystem::path& inSourcePath);

	void InitializeFromMeshData(const List<SerializationMeshData>& inMeshDatas);
	void InitializeStagingBuffers(List<SerializationMeshData>& inMeshDatas);
	uint GetRequiredVertexBufferSize(const List<SerializationMeshData>& inMeshDatas);
	

private:
	List<Mesh*> myMeshes{};

};