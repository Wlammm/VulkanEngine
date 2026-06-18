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

	// True once PostPropertiesSerialized has created this model's GPU resources (vertex/index/mesh
	// buffers, BLAS). Until then GetMeshes() is empty and the handles are not valid for rendering.
	// Consumers loaded asynchronously may receive this model before that happens (PostPropertiesSerialized
	// is deferred to the main thread), so they must gate rendering on this flag.
	bool IsRenderReady() const { return myIsRenderReady; }

	// TODO: We currently have mesh datas always loaded. But in the future it might cost too much memory so find solution to load on demand instead. This is needed for mesh colliders.
	const List<SerializationMeshData>& GetSerializationMeshDatas() const;

	const List<BLAS*>& GetBLASes() const;

private:
	List<Mesh*> myMeshes{};
	List<BLAS*> myBLASes{};

	// Set true at the end of PostPropertiesSerialized once GPU resources exist. Runtime-only, not serialized.
	bool myIsRenderReady = false;

	META(SerializeField)
	List<SerializationMeshData> myMeshDatas{};
};