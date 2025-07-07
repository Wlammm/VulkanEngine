#pragma once

#include "Engine/AssetRegistry/Asset.h"
#include "Engine/Rendering/Vertex.hpp"

class VulkanBuffer;
class Mesh;

using CachePath = std::filesystem::path;
using SourcePath = std::filesystem::path;

struct SerializationMeshData
{
	META(SerializeField)
	List<Vertex> myVertices{};
	META(SerializeField)
	List<uint> myIndices{};
	META(SerializeField)
	glm::vec4 mySphereCenterBounds{};
	VulkanBuffer* myStagingVertexBuffer = nullptr;
	VulkanBuffer* myStagingIndexBuffer = nullptr;
	META(SerializeField)
	std::string myAlbedoPath;
	META(SerializeField)
	std::string myNormalPath;
	META(SerializeField)
	std::string myMaterialPath;
};

struct SerializationModelData
{
	META(SerializeField)
	std::filesystem::path mySourceFilePath;

	META(SerializeField)
	std::filesystem::file_time_type myLastCachedWriteTime;
	
	META(SerializeField)
	List<SerializationMeshData> myMeshDatas{};
};

class Model : public Asset
{
	static constexpr uint BinaryVersion = 1;
	
public:
	Coroutine<void, void, false> Load(const std::filesystem::path inPath) override;
	void Unload() override;

	const List<Mesh*>& GetMeshes() const;

	// Used to query vertex information about a model.
	static SerializationModelData GetSerializationDataForModel(Model* inModel);
	
private:
	static CachePath ConvertToCachePath(const SourcePath& inPath);
	static bool IsCached(const CachePath& inPath);

	static bool TryLoadModelDataFromCache(const CachePath& inPath, SerializationModelData& outModelData);
	static void SaveModelDataToCache(const CachePath& inPath, SerializationModelData& inModelData);
	
	static SerializationModelData LoadModelDataFromSourceFile(const SourcePath& inPath);

	void InitializeFromMeshData(const SerializationModelData& inModelData);
	void InitializeStagingBuffers(SerializationModelData& inModelData);
	uint GetRequiredVertexBufferSize(const SerializationModelData& inModelData);
	
private:
	List<Mesh*> myMeshes{};
};