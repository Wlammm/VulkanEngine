#include "EnginePch.h"
#include "Model.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "Engine.h"
#include "Coroutines/Awaitable.h"
#include "Rendering/IndexBufferSystem.h"
#include "Rendering/Mesh.h"
#include "Rendering/MeshSystem.h"
#include "Rendering/MeshUtils.h"
#include "Rendering/VertexBufferSystem.h"
#include "Serialization/BinaryReader.h"
#include "Serialization/BinaryWriter.h"
#include "Vulkan/VulkanAllocator.h"
#include "Vulkan/VulkanBuffer.h"
#include "Vulkan/VulkanContext.h"

Coroutine<void, void, false> Model::Load(const std::filesystem::path inPath)
{
	List<SerializationMeshData> meshDatas;
	{
		ZoneScoped;
	
		bool shouldRecache = true;
		if(IsCached(inPath))
		{
			shouldRecache = !TryLoadMeshDatasFromCache(inPath, meshDatas);
		}
	
		if(shouldRecache)
		{
			meshDatas = LoadMeshDatasFromFbx(inPath);
			SaveToCache(meshDatas, inPath);
		}
	}
	InitializeStagingBuffers(meshDatas);
	
	co_await Awaitables::SwitchToThread(ThreadType::Main);
	{
		ZoneScoped;
		InitializeFromMeshData(meshDatas);
	}

	co_await Awaitables::SwitchToThread(ThreadType::Worker);
	{
		meshDatas.Reset();
	}
}

void Model::Unload()
{
	//ZoneScoped;
}

const List<Mesh*>& Model::GetMeshes() const
{
	return myMeshes;
}

std::filesystem::path Model::GetCachedFilePath(const std::filesystem::path& inPath)
{
	return L"Cache/ModelCache/" + inPath.filename().wstring() + L".model";
}

bool Model::IsCached(const std::filesystem::path& inPath)
{
	return std::filesystem::exists(GetCachedFilePath(inPath));
}

bool Model::TryLoadMeshDatasFromCache(const CachePath& inPath, List<SerializationMeshData>& outMeshDatas)
{
	//ZoneScoped;
	BinaryReader reader(inPath);

	int fileVersion;
	reader.Read(fileVersion);

	if(fileVersion != BinaryVersion)
	{
		LOG_WARNING("ModelFactory: Outdated binary for model: %s", inPath.string().c_str());
		return false;
	}

	std::filesystem::path sourceFilePath;
	reader.Read(sourceFilePath);
	
	std::filesystem::file_time_type lastWriteTimeSourceFile;
	reader.Read(lastWriteTimeSourceFile);

	const std::filesystem::file_time_type fbxLastWriteTime = std::filesystem::last_write_time(sourceFilePath);
	if (!std::filesystem::exists(sourceFilePath))
	{
		LOG_WARNING("Model: Source file does not exists anymore.", inPath.string().c_str());
		return false;
	}

	if (fbxLastWriteTime != lastWriteTimeSourceFile)
	{
		LOG_WARNING("Model: Cache is out of date. Model not loaded: %s", inPath.string().c_str());
		return false;
	}

	size_t numMeshes;
	reader.Read(numMeshes);

	for (size_t i = 0; i < numMeshes; ++i)
	{
		SerializationMeshData& mesh = outMeshDatas.Emplace();
		reader.Read(mesh.myVertices);
		reader.Read(mesh.myIndices);
		reader.Read(mesh.mySphereCenterBounds);
	}
	return true;
}

List<SerializationMeshData> Model::LoadMeshDatasFromFbx(const std::filesystem::path& inPath)
{
	//ZoneScoped;
	List<SerializationMeshData> meshDatas{};
	
	static thread_local Assimp::Importer myImporter{};
	
	uint flags = aiProcessPreset_TargetRealtime_MaxQuality |
		//aiProcess_ConvertToLeftHanded |
		aiProcess_Triangulate |
		aiProcess_CalcTangentSpace |
		aiProcess_SortByPType |
		aiProcess_PreTransformVertices |
		aiProcess_FlipWindingOrder;
	flags &= ~aiProcess_JoinIdenticalVertices;

	myImporter.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_LINE | aiPrimitiveType_POINT);
	const aiScene* scene = myImporter.ReadFile(inPath.string(), flags);

	if (!scene)
	{
		LOG_ERROR("Failed to load model: %s", inPath.string().c_str());
		return meshDatas;
	}

	const uint numMeshes = scene->mNumMeshes;
	if (numMeshes == 0)
	{
		LOG_ERROR("FBX does not contain any meshes: %s", inPath.string().c_str());
		return meshDatas;
	}
	
	for (uint meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex)
	{
		SerializationMeshData& meshData = meshDatas.Emplace();

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
	return meshDatas;
}

void Model::SaveToCache(const List<SerializationMeshData>& inMeshDatas, const std::filesystem::path& inSourcePath)
{
	//ZoneScoped;
	BinaryWriter writer(GetCachedFilePath(inSourcePath));
	writer.Write(BinaryVersion);

	writer.Write(inSourcePath);
	const std::filesystem::file_time_type fbxLastWriteTime = std::filesystem::last_write_time(inSourcePath);
	writer.Write(fbxLastWriteTime);

	const size_t numMeshes = inMeshDatas.size();
	writer.Write(numMeshes);

	for(const SerializationMeshData& mesh : inMeshDatas)
	{
		writer.Write(mesh.myVertices);
		writer.Write(mesh.myIndices);
		writer.Write(mesh.mySphereCenterBounds);
	}

	writer.Save();
}

void Model::InitializeFromMeshData(const List<SerializationMeshData>& inMeshDatas)
{
	//ZoneScoped;
	for(const SerializationMeshData& meshData : inMeshDatas)
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
void Model::InitializeStagingBuffers(List<SerializationMeshData>& inMeshDatas)
{
	for(SerializationMeshData& meshData : inMeshDatas)
	{
		const uint vertexSize = meshData.myVertices.size() * sizeof(Vertex);
		meshData.myStagingVertexBuffer = VulkanAllocator::AllocateBuffer_TS("ModelVertexBufferStaging", VulkanBuffer::StagingCreateInfo(vertexSize), VMA_MEMORY_USAGE_AUTO, true);
		meshData.myStagingVertexBuffer->SetData(meshData.myVertices.data(), vertexSize);

		const uint indexSize = meshData.myIndices.size() * sizeof(uint);
		meshData.myStagingIndexBuffer = VulkanAllocator::AllocateBuffer_TS("ModelIndexBufferStaging", VulkanBuffer::StagingCreateInfo(indexSize), VMA_MEMORY_USAGE_AUTO, true);
		meshData.myStagingIndexBuffer->SetData(meshData.myIndices.data(), indexSize);
	}
}

uint Model::GetRequiredVertexBufferSize(const List<SerializationMeshData>& inMeshDatas)
{
	uint requiredSize = 0;
	for(const SerializationMeshData& meshData : inMeshDatas)
	{
		requiredSize += sizeof(Vertex) * meshData.myVertices.size();
	}
	return requiredSize;
}