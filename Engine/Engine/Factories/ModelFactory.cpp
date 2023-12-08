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
