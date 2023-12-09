#include "EnginePch.h"
#include "ModelFactory.h"
#include "Rendering/Vertex.hpp"


#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include "Rendering/Mesh.h"
#include "Vulkan/VulkanVertexBuffer.h"
#include "Vulkan/VulkanIndexBuffer.h"
#include "Engine.h"
#include "Assets/Material.h"
#include "Assets/AssetRegistry.h"

#include "Utils/BinaryUtils.hpp"
#include "Serialization/BinaryWriter.h"
#include "Serialization/BinaryReader.h"

Model* ModelFactory::LoadModelFromFbx(const std::filesystem::path& inPath)
{
	static thread_local Assimp::Importer myImporter{};

	uint flags = aiProcessPreset_TargetRealtime_MaxQuality |
		//aiProcess_ConvertToLeftHanded |
		aiProcess_Triangulate |
		aiProcess_CalcTangentSpace |
		aiProcess_SortByPType |
		aiProcess_PreTransformVertices;
	flags &= ~aiProcess_JoinIdenticalVertices;

	myImporter.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_LINE | aiPrimitiveType_POINT);
	const aiScene* scene = myImporter.ReadFile(inPath.string(), flags);

	if (!scene)
	{
		LOG_ERROR("Failed to load model: %s", inPath.string().c_str());
		return nullptr;
	}

	const uint numMeshes = scene->mNumMeshes;
	if (numMeshes == 0)
	{
		LOG_ERROR("FBX does not contain any meshes: %s", inPath.string().c_str());
		return nullptr;
	}

	List<Mesh> meshes{};
	for (uint meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex)
	{
		List<Vertex> vertices{};
		List<uint> indices{};

		Mesh mesh{};
		const aiMesh* aiMesh = scene->mMeshes[meshIndex];

		// This has not been fully implemented. Its just here to show how to get material textures for now :) 
		aiMaterial* aiMat = scene->mMaterials[aiMesh->mMaterialIndex];
		aiString aiAlbedoPath;
		aiMat->GetTexture(aiTextureType_BASE_COLOR, 0, &aiAlbedoPath);

		mesh.NumVertices = aiMesh->mNumVertices;
		mesh.NumIndices = aiMesh->mNumFaces * 3;

		for (uint vertexIndex = 0; vertexIndex < mesh.NumVertices; ++vertexIndex)
		{
			Vertex vertex{};

			// Import models with inverted y due to some error in the matrices that I cant be asked looking into right now ¯\_(ö)_/¯
			vertex.myPosition = { aiMesh->mVertices[vertexIndex].x, -aiMesh->mVertices[vertexIndex].y, aiMesh->mVertices[vertexIndex].z, 1.0f };
			vertex.myColor = { 1, 1, 1, 1 };

			if (aiMesh->GetNumColorChannels() > 0)
				vertex.myColor = { aiMesh->mColors[0][vertexIndex].r, aiMesh->mColors[0][vertexIndex].g, aiMesh->mColors[0][vertexIndex].b, aiMesh->mColors[0][vertexIndex].a };
			aiMesh->mNumBones;
			if (aiMesh->mNormals)
				vertex.myNormal = { aiMesh->mNormals[vertexIndex].x, aiMesh->mNormals[vertexIndex].y, aiMesh->mNormals[vertexIndex].z, 0.0f };

			if (aiMesh->mTangents)
				vertex.myTangents = { aiMesh->mTangents[vertexIndex].x,aiMesh->mTangents[vertexIndex].y, aiMesh->mTangents[vertexIndex].z, 0.0f };

			if (aiMesh->mBitangents)
				vertex.myBinormals = { aiMesh->mBitangents[vertexIndex].x, aiMesh->mBitangents[vertexIndex].y, aiMesh->mBitangents[vertexIndex].z, 0.0f };

			for (uint texCoordIndex = 0; texCoordIndex < 4; ++texCoordIndex)
			{
				if (aiMesh->mTextureCoords[texCoordIndex])
					vertex.myTexCoords[texCoordIndex] = { aiMesh->mTextureCoords[texCoordIndex][vertexIndex].x, aiMesh->mTextureCoords[texCoordIndex][vertexIndex].y };
			}

			vertices.Add(vertex);
		}

		uint faceIndex = 0;
		for (uint indicesIndex = 0; indicesIndex < mesh.NumIndices;)
		{
			for (uint index = 0; index < 3; ++index)
			{
				indices.Add(aiMesh->mFaces[faceIndex].mIndices[index]);
				indicesIndex++;
			}
			faceIndex++;
		}

		// Everything below needs to be handled in a thread safe way before we async load this.
		mesh.VertexBuffer = new VulkanVertexBuffer(vertices);
		mesh.IndexBuffer = new VulkanIndexBuffer(indices);

		const std::filesystem::path* albedoPath = Engine::GetAssetRegistry().TryGetPathFromFilename(aiAlbedoPath.C_Str());
		if (albedoPath)
		{
			MaterialCreateInfo createInfo;
			createInfo.myAlbedoPath = *albedoPath;
			createInfo.myNormalPath = *albedoPath;
			createInfo.myMaterialPath = *albedoPath;
			mesh.myMaterial = Engine::GetAssetRegistry().GetMaterial(createInfo);
		}
		else
			mesh.myMaterial = Engine::GetAssetRegistry().GetDefaultMaterial();

		meshes.Add(mesh);
	}

	myImporter.FreeScene();

	return new Model(meshes);
}

void ModelFactory::SaveModelDataToBinary(const ModelData& inData, const std::filesystem::path& inSavePath)
{
	BinaryWriter writer(GetCachedFilePath(inData.mySourceFile));
	writer.Write(BinaryVersion);

	writer.Write(inData.mySourceFile);
	const std::filesystem::file_time_type fbxLastWriteTime = std::filesystem::last_write_time(inData.mySourceFile);
	writer.Write(fbxLastWriteTime);

	const size_t numMeshes = inData.myMeshes.size();
	writer.Write(numMeshes);

	for(const MeshData& mesh : inData.myMeshes)
	{
		writer.Write(mesh.myVertices);
		writer.Write(mesh.myIndices);
		writer.Write(mesh.myAlbedoPath);
		writer.Write(mesh.myNormalPath);
		writer.Write(mesh.myMaterialPath);
	}

	writer.Save();
}

ModelFactory::ModelData ModelFactory::GetModelDataFromBinary(const std::filesystem::path& inPath)
{
	ModelData modelData;
	BinaryReader reader(inPath);

	int fileVersion;
	reader.Read(fileVersion);

	if(fileVersion != BinaryVersion)
	{
		LOG_WARNING("ModelFactory: Outdated binary for model: %s", inPath.string().c_str());
		return modelData;
	}

	reader.Read(modelData.mySourceFile);
	
	std::filesystem::file_time_type lastWriteTimeSourceFile;
	reader.Read(lastWriteTimeSourceFile);

	const std::filesystem::file_time_type fbxLastWriteTime = std::filesystem::last_write_time(modelData.mySourceFile);
	if(!std::filesystem::exists(modelData.mySourceFile))
	{
		LOG_WARNING("ModelFactory: Source file does not exists anymore.", inPath.string().c_str());
		return modelData;
	}

	if(fbxLastWriteTime != lastWriteTimeSourceFile)
	{
		LOG_WARNING("ModelFactory: Source file has been updated: %s", inPath.string().c_str());
		return modelData;
	}

	size_t numMeshes;
	reader.Read(numMeshes);

	for(size_t i = 0; i <numMeshes; ++i)
	{
		MeshData& mesh = modelData.myMeshes.Emplace();
		reader.Read(mesh.myVertices);
		reader.Read(mesh.myIndices);
		reader.Read(mesh.myAlbedoPath);
		reader.Read(mesh.myNormalPath);
		reader.Read(mesh.myMaterialPath);
	}

	return modelData;
}

Model* ModelFactory::CreateModelFromModelData(const ModelData& inModelData)
{
	List<Mesh> meshes{};

	for(const MeshData& meshData : inModelData.myMeshes)
	{
		Mesh& mesh = meshes.Emplace();

		// Everything below needs to be handled in a thread safe way before we async load this.
		mesh.VertexBuffer = new VulkanVertexBuffer(meshData.myVertices);
		mesh.IndexBuffer = new VulkanIndexBuffer(meshData.myIndices);

		if(std::filesystem::exists(meshData.myAlbedoPath))
		{
			MaterialCreateInfo createInfo;
			createInfo.myAlbedoPath = meshData.myAlbedoPath;
			createInfo.myNormalPath = meshData.myAlbedoPath;
			createInfo.myMaterialPath = meshData.myAlbedoPath;
			mesh.myMaterial = Engine::GetAssetRegistry().GetMaterial(createInfo);
		}
		else
		{
			mesh.myMaterial = Engine::GetAssetRegistry().GetDefaultMaterial();
		}
	}

	Model* model = new Model(meshes);
	return model;
}

ModelFactory::ModelData ModelFactory::GetModelDataFromFbx(const std::filesystem::path& inPath)
{
	ModelData modelData;
	modelData.mySourceFile = inPath;

	static thread_local Assimp::Importer myImporter{};

	uint flags = aiProcessPreset_TargetRealtime_MaxQuality |
		//aiProcess_ConvertToLeftHanded |
		aiProcess_Triangulate |
		aiProcess_CalcTangentSpace |
		aiProcess_SortByPType |
		aiProcess_PreTransformVertices;
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

	List<Mesh> meshes{};
	for (uint meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex)
	{
		List<Vertex> vertices{};
		List<uint> indices{};

		Mesh mesh{};
		const aiMesh* aiMesh = scene->mMeshes[meshIndex];

		// This has not been fully implemented. Its just here to show how to get material textures for now :) 
		aiMaterial* aiMat = scene->mMaterials[aiMesh->mMaterialIndex];
		aiString aiAlbedoPath;
		aiMat->GetTexture(aiTextureType_BASE_COLOR, 0, &aiAlbedoPath);

		mesh.NumVertices = aiMesh->mNumVertices;
		mesh.NumIndices = aiMesh->mNumFaces * 3;

		for (uint vertexIndex = 0; vertexIndex < mesh.NumVertices; ++vertexIndex)
		{
			Vertex vertex{};

			// Import models with inverted y due to some error in the matrices that I cant be asked looking into right now ¯\_(ö)_/¯
			vertex.myPosition = { aiMesh->mVertices[vertexIndex].x, -aiMesh->mVertices[vertexIndex].y, aiMesh->mVertices[vertexIndex].z, 1.0f };
			vertex.myColor = { 1, 1, 1, 1 };

			if (aiMesh->GetNumColorChannels() > 0)
				vertex.myColor = { aiMesh->mColors[0][vertexIndex].r, aiMesh->mColors[0][vertexIndex].g, aiMesh->mColors[0][vertexIndex].b, aiMesh->mColors[0][vertexIndex].a };
			aiMesh->mNumBones;
			if (aiMesh->mNormals)
				vertex.myNormal = { aiMesh->mNormals[vertexIndex].x, aiMesh->mNormals[vertexIndex].y, aiMesh->mNormals[vertexIndex].z, 0.0f };

			if (aiMesh->mTangents)
				vertex.myTangents = { aiMesh->mTangents[vertexIndex].x,aiMesh->mTangents[vertexIndex].y, aiMesh->mTangents[vertexIndex].z, 0.0f };

			if (aiMesh->mBitangents)
				vertex.myBinormals = { aiMesh->mBitangents[vertexIndex].x, aiMesh->mBitangents[vertexIndex].y, aiMesh->mBitangents[vertexIndex].z, 0.0f };

			for (uint texCoordIndex = 0; texCoordIndex < 4; ++texCoordIndex)
			{
				if (aiMesh->mTextureCoords[texCoordIndex])
					vertex.myTexCoords[texCoordIndex] = { aiMesh->mTextureCoords[texCoordIndex][vertexIndex].x, aiMesh->mTextureCoords[texCoordIndex][vertexIndex].y };
			}

			vertices.Add(vertex);
		}

		uint faceIndex = 0;
		for (uint indicesIndex = 0; indicesIndex < mesh.NumIndices;)
		{
			for (uint index = 0; index < 3; ++index)
			{
				indices.Add(aiMesh->mFaces[faceIndex].mIndices[index]);
				indicesIndex++;
			}
			faceIndex++;
		}

		const std::filesystem::path* albedoPath = Engine::GetAssetRegistry().TryGetPathFromFilename(aiAlbedoPath.C_Str());

		MeshData& meshData = modelData.myMeshes.Emplace();
		meshData.myVertices = vertices;
		meshData.myIndices = indices;
		meshData.myAlbedoPath = *albedoPath;
		meshData.myNormalPath = *albedoPath;
		meshData.myMaterialPath = *albedoPath;
	}

	myImporter.FreeScene();

	return modelData;
}


std::filesystem::path ModelFactory::GetCachedFilePath(const std::filesystem::path& inPath)
{
	std::filesystem::path cachePath = L"Cache/ModelCache/" + inPath.filename().wstring() + L".model";
	return cachePath;
}
