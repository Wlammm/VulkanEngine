#include "EnginePch.h"
#include "Model.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "Engine/Engine.h"
#include "Engine/Coroutines/Awaitable.h"
#include "Engine/Reflection/Class.h"
#include "Engine/Reflection/ReflectionSystem.h"
#include "Engine/Rendering/IndexBufferSystem.h"
#include "Engine/Rendering/Mesh.h"
#include "Engine/Rendering/MeshSystem.h"
#include "Engine/Rendering/MeshUtils.h"
#include "Engine/Rendering/VertexBufferSystem.h"
#include "Engine/Serialization/BinarySerializer.h"
#include "Engine/Vulkan/VulkanAllocator.h"
#include "Engine/Vulkan/VulkanBuffer.h"
#include "Engine/Vulkan/VulkanContext.h"

Coroutine<void, void, false> Model::Load(const std::filesystem::path inPath)
{
	SerializationModelData modelData;
	{
		ZoneScoped;
		const CachePath cachedPath = ConvertToCachePath(inPath);
		if (!TryLoadModelDataFromCache(cachedPath, modelData))
		{
			modelData = LoadModelDataFromSourceFile(inPath);
			SaveModelDataToCache(cachedPath, modelData);
		}
	}
	InitializeStagingBuffers(modelData);
	
	co_await Awaitables::SwitchToThread(ThreadType::Main);
	{
		ZoneScoped;
		InitializeFromMeshData(modelData);
	}

	co_await Awaitables::SwitchToThread(ThreadType::Worker);
}

void Model::Unload()
{
	//ZoneScoped;
}

const List<Mesh*>& Model::GetMeshes() const
{
	return myMeshes;
}

SerializationModelData Model::GetSerializationDataForModel(Model* inModel)
{
	ZoneScoped;
	check(inModel);
	
	SerializationModelData modelData;
	{
		ZoneScoped;
		if (!TryLoadModelDataFromCache(inModel->myPath, modelData))
		{
			modelData = LoadModelDataFromSourceFile(inModel->myPath);
			SaveModelDataToCache(inModel->myPath, modelData);
		}
	}
	
	return modelData;
}

std::filesystem::path Model::ConvertToCachePath(const std::filesystem::path& inPath)
{
	return L"Cache/ModelCache/" + inPath.filename().wstring() + L".model";
}

bool Model::IsCached(const CachePath& inPath)
{
	return std::filesystem::exists(ConvertToCachePath(inPath));
}

bool Model::TryLoadModelDataFromCache(const CachePath& inPath, SerializationModelData& outModelData)
{
	if (!std::filesystem::exists(ConvertToCachePath(inPath)))
		return false;

	BinarySerializer serializer(inPath, BinarySerializer::Mode::Read);
	
	SerializationModelData modelData;
	serializer.SerializeClass(modelData);

	// Structure has failed and some fields failed to serialize correctly. 
	if (!serializer.WasLastClassSerializationFullyComplete())
		return false;

	check(std::filesystem::exists(modelData.mySourceFilePath) && "Attempting to access cached model data with deleted source asset.");
	const std::filesystem::file_time_type sourceLastWriteTime = std::filesystem::last_write_time(modelData.mySourceFilePath);

	if (sourceLastWriteTime != modelData.myLastCachedWriteTime)
		return false;

	outModelData = std::move(modelData);
	return true;
}

void Model::SaveModelDataToCache(const std::filesystem::path& inPath, SerializationModelData& inModelData)
{
	BinarySerializer serializer(inPath, BinarySerializer::Mode::Write);
	serializer.SerializeClass(inModelData);
}

SerializationModelData Model::LoadModelDataFromSourceFile(const std::filesystem::path& inPath)
{
	//ZoneScoped;
	SerializationModelData modelData{};
	modelData.mySourceFilePath = inPath;
	
	static thread_local Assimp::Importer myImporter{};
	
	uint flags = aiProcessPreset_TargetRealtime_MaxQuality |
		//aiProcess_ConvertToLeftHanded |
		aiProcess_Triangulate |
		aiProcess_CalcTangentSpace |
		aiProcess_SortByPType |
		aiProcess_PreTransformVertices;// |
		// aiProcess_FlipWindingOrder;
	flags &= ~aiProcess_JoinIdenticalVertices;

	myImporter.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_LINE | aiPrimitiveType_POINT);
	const aiScene* scene = myImporter.ReadFile(inPath.string(), flags);

	if (!scene)
	{
		LOG_ERROR("Failed to load model: %s", inPath.string().c_str());
		return modelData;
	}

	const uint numMeshes = scene->mNumMeshes;
	if (numMeshes == 0)
	{
		LOG_ERROR("FBX does not contain any meshes: %s", inPath.string().c_str());
		return modelData;
	}
	
	for (uint meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex)
	{
		SerializationMeshData& meshData = modelData.myMeshDatas.Emplace();

		const aiMesh* aiMesh = scene->mMeshes[meshIndex];
		
		uint numVertices = aiMesh->mNumVertices;
		uint numIndices = aiMesh->mNumFaces * 3;

		for (uint vertexIndex = 0; vertexIndex < numVertices; ++vertexIndex)
		{
			Vertex vertex{};

			vertex.myPosition = { aiMesh->mVertices[vertexIndex].x, aiMesh->mVertices[vertexIndex].y, aiMesh->mVertices[vertexIndex].z };

			if (aiMesh->GetNumColorChannels() > 0)
			{
				vertex.PackColor({aiMesh->mColors[0][vertexIndex].r, aiMesh->mColors[0][vertexIndex].g, aiMesh->mColors[0][vertexIndex].b, aiMesh->mColors[0][vertexIndex].a});
			}
			
			if (aiMesh->mNormals)
				vertex.myNormal = { aiMesh->mNormals[vertexIndex].x, aiMesh->mNormals[vertexIndex].y, aiMesh->mNormals[vertexIndex].z };

			if (aiMesh->mTangents)
				vertex.myTangents = { aiMesh->mTangents[vertexIndex].x, aiMesh->mTangents[vertexIndex].y, aiMesh->mTangents[vertexIndex].z };

			if (aiMesh->mBitangents)
				vertex.myBinormals = { aiMesh->mBitangents[vertexIndex].x, aiMesh->mBitangents[vertexIndex].y, aiMesh->mBitangents[vertexIndex].z };

			for (uint texCoordIndex = 0; texCoordIndex < 2; ++texCoordIndex)
			{
				if (aiMesh->mTextureCoords[texCoordIndex])
					vertex.myTexCoords[texCoordIndex] = { aiMesh->mTextureCoords[texCoordIndex][vertexIndex].x, -aiMesh->mTextureCoords[texCoordIndex][vertexIndex].y };
			}

			meshData.myVertices.Add(vertex);
		}

		uint faceIndex = 0;
		for (uint indicesIndex = 0; indicesIndex < numIndices;)
		{
			for (uint index = 0; index < 3; ++index)
			{
				meshData.myIndices.Add(aiMesh->mFaces[faceIndex].mIndices[index]);
				indicesIndex++;
			}
			faceIndex++;
		}

		// Get the material for the mesh
		if (aiMesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[aiMesh->mMaterialIndex];

			aiString albedoPath;
			material->GetTexture(aiTextureType_BASE_COLOR, 0, &albedoPath);
			aiString normalPath;
			material->GetTexture(aiTextureType_NORMALS, 0, &normalPath);
			aiString materialPath;
			material->GetTexture(aiTextureType_METALNESS, 0, &materialPath);

			meshData.myAlbedoPath = albedoPath.C_Str();
			meshData.myNormalPath = normalPath.C_Str();
			meshData.myMaterialPath = materialPath.C_Str();
		}
		
		meshData.mySphereCenterBounds = MeshUtils::CalculateSphereBounds(meshData.myVertices);
	}

	myImporter.FreeScene();
	return modelData;
}

void Model::InitializeFromMeshData(const SerializationModelData& inModelData)
{
	//ZoneScoped;
	for(const SerializationMeshData& meshData : inModelData.myMeshDatas)
	{
		VertexBufferHandle* vertexBuffer = Engine::GetEngineSystem<VertexBufferSystem>().UploadVertexBuffer(meshData.myStagingVertexBuffer, meshData.myVertices.size());
		VulkanAllocator::DestroyBuffer_TS(meshData.myStagingVertexBuffer);
		
		IndexBufferHandle* indexBuffer = Engine::GetEngineSystem<IndexBufferSystem>().UploadIndexBuffer(meshData.myStagingIndexBuffer, meshData.myIndices.size());
		VulkanAllocator::DestroyBuffer_TS(meshData.myStagingIndexBuffer);

		const glm::vec4 boudingSphere = meshData.mySphereCenterBounds;

		myMeshes.Add(Engine::GetEngineSystem<MeshSystem>().UploadMesh(vertexBuffer, indexBuffer, boudingSphere));
		myMeshes.Last()->myAlbedoPath = meshData.myAlbedoPath;
		myMeshes.Last()->myNormalPath = meshData.myNormalPath;
		myMeshes.Last()->myMaterialPath = meshData.myMaterialPath;
	}
}

void Model::InitializeStagingBuffers(SerializationModelData& inModelData)
{
	for(SerializationMeshData& meshData : inModelData.myMeshDatas)
	{
		const uint vertexSize = meshData.myVertices.size() * sizeof(Vertex);
		meshData.myStagingVertexBuffer = VulkanAllocator::AllocateBuffer_TS("ModelVertexBufferStaging", VulkanBuffer::StagingCreateInfo(vertexSize), VMA_MEMORY_USAGE_AUTO, true);
		meshData.myStagingVertexBuffer->SetData(meshData.myVertices.data(), vertexSize);

		const uint indexSize = meshData.myIndices.size() * sizeof(uint);
		meshData.myStagingIndexBuffer = VulkanAllocator::AllocateBuffer_TS("ModelIndexBufferStaging", VulkanBuffer::StagingCreateInfo(indexSize), VMA_MEMORY_USAGE_AUTO, true);
		meshData.myStagingIndexBuffer->SetData(meshData.myIndices.data(), indexSize);
	}
}

uint Model::GetRequiredVertexBufferSize(const SerializationModelData& inModelData)
{
	uint requiredSize = 0;
	for(const SerializationMeshData& meshData : inModelData.myMeshDatas)
	{
		requiredSize += sizeof(Vertex) * meshData.myVertices.size();
	}
	return requiredSize;
}