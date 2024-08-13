#pragma once
#include "Rendering/Vertex.hpp"

class Model;

// This does not work as intended yet. Just remove the cached modeldata folder for now.
constexpr int BinaryVersion = 1;

// DONT USE THIS CLASS DIRECTLY. USE THE ASSET REGISTRIES INSTEAD.
class ModelFactory
{
	struct MeshSerializationData
	{
		List<Vertex> myVertices{};
		List<uint> myIndices{};
		glm::vec4 mySphereCenterBounds{};
		std::filesystem::path myAlbedoPath = "";
		std::filesystem::path myNormalPath = "";
		std::filesystem::path myMaterialPath = "";
	};

	struct ModelData
	{
		// This is the path to the file which the data has been loaded from. Fbx or other supported format.
		std::filesystem::path mySourceFile;
		List<MeshSerializationData> myMeshes{};
	};

public:
	static Model* GetModel(const std::filesystem::path& inPath);

private:
	static void GetModelDataFromFbx(const std::filesystem::path& inPath, ModelData& outData);

	static void SaveModelDataToBinary(const ModelData& inModelData, const std::filesystem::path& inSavePath);
	static void GetModelDataFromBinary(const std::filesystem::path& inPath, ModelData& outData);

	static Model* CreateModelFromModelData(const ModelData& inModelData);

	static std::filesystem::path GetCachedFilePath(const std::filesystem::path& inPath);

	static glm::vec4 CalculateSphereBounds(const List<Vertex>& inVertices);
};