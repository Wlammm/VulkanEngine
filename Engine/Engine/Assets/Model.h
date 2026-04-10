#pragma once

#include "Engine/AssetRegistry/Asset.h"
#include "Engine/Rendering/Vertex.hpp"

class IndexBufferHandle;
class VertexBufferHandle;
class VulkanBuffer;
class Mesh;
class BLAS;

struct SerializationMeshData
{
	META(SerializeField)
	List<Vertex> myVertices{};

	META(SerializeField)
	List<uint> myIndices{};

	META(SerializeField)
	glm::vec4 mySphereCenterBounds{};

	META(SerializeField)
	std::string myAlbedoPath;

	META(SerializeField)
	std::string myNormalPath;

	META(SerializeField)
	std::string myMaterialPath;
};

class Model : public Asset
{
public:
	~Model();
	
	static List<std::string> GetAssetExtensions() { return {".fbx", ".gltf" }; }
	
	void PostPropertiesSerialized() override;

	void LoadPropertiesFromSource() override;
	
	static constexpr bool IsExternalAsset() { return true; };
	
	const List<Mesh*>& GetMeshes() const;

	// TODO: We currently have mesh datas always loaded. But in the future it might cost too much memory so find solution to load on demand instead. This is needed for mesh colliders.
	const List<SerializationMeshData>& GetSerializationMeshDatas() const;

	const List<BLAS*>& GetBLASes() const;

private:
	List<Mesh*> myMeshes{};
	List<BLAS*> myBLASes{};

	META(SerializeField)
	List<SerializationMeshData> myMeshDatas{};
};