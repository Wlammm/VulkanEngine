#include "EnginePch.h"
#include "Model.h"
#include "Rendering/Vertex.hpp"
#include "Vulkan/VulkanVertexBuffer.h"
#include "Vulkan/VulkanContext.h"
#include "Vulkan/VulkanDevice.h"
#include "Vulkan/VulkanIndexBuffer.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include "Engine.h"
#include "Assets/AssetRegistry.h"
#include "Assets/Material.h"

Model::Model()
{
	Mesh mesh;
	List<Vertex> vertices
	{
		{ { -0.5f,  0.5f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }},
		{ {	 0.5f,  0.5f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }},
		{ {	 0.0f, -0.5f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }},
	};
	mesh.VertexBuffer = new VulkanVertexBuffer(vertices);

	List<uint> indices
	{
		0, 1, 2 
	};
	mesh.IndexBuffer = new VulkanIndexBuffer(indices);
	myMeshes.Add(mesh);

	myIsValid = true;
}

Model::Model(const std::filesystem::path& inPath, const CreateInfo& inCreateInfo)
	: myPath{ inPath }
{
	bool flipIndices = false;

	if (inCreateInfo.InvertY)
		flipIndices = !flipIndices;
	if (inCreateInfo.FlipIndicies)
		flipIndices = !flipIndices;

	Assimp::Importer importer;

	uint flags = aiProcessPreset_TargetRealtime_MaxQuality |
		//aiProcess_ConvertToLeftHanded |
		aiProcess_Triangulate |
		aiProcess_CalcTangentSpace |
		aiProcess_SortByPType |
		aiProcess_PreTransformVertices;
	flags &= ~aiProcess_JoinIdenticalVertices;

	importer.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_LINE | aiPrimitiveType_POINT);

	const aiScene* scene = importer.ReadFile(inPath.string(), flags);

	if(!scene)
	{
		LOG_ERROR("Failed to load model");
		return;
	}

	const uint numMeshes = scene->mNumMeshes;
	if(numMeshes == 0)
	{
		LOG_ERROR("FBX does not contain any meshes.");
		return;
	}

	for (uint meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex)
	{
		List<Vertex> vertices{};
		List<uint> indices{};

		Mesh mesh{};
		const aiMesh* aiMesh = scene->mMeshes[meshIndex];
		
		// This has not been fully implemented. Its just here to show how to get material textures for now :) 
		aiMaterial* aiMat = scene->mMaterials[aiMesh->mMaterialIndex];
		aiString string;
		aiMat->GetTexture(aiTextureType_BASE_COLOR, 0, &string);

		mesh.NumVertices = aiMesh->mNumVertices;
		mesh.NumIndices = aiMesh->mNumFaces * 3;

		for (uint vertexIndex = 0; vertexIndex < mesh.NumVertices; ++vertexIndex)
		{
			Vertex vertex{};
			vertex.myPosition = { aiMesh->mVertices[vertexIndex].x, inCreateInfo.InvertY ? -aiMesh->mVertices[vertexIndex].y : aiMesh->mVertices[vertexIndex].y, aiMesh->mVertices[vertexIndex].z, 1.0f };
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
				indices.Add(aiMesh->mFaces[faceIndex].mIndices[flipIndices ? 2 - index : index]);
				indicesIndex++;
			}
			faceIndex++;
		}

		mesh.VertexBuffer = new VulkanVertexBuffer(vertices);
		mesh.IndexBuffer = new VulkanIndexBuffer(indices);
		mesh.myMaterial = Engine::GetAssetRegistry().GetDefaultMaterial();
		myMeshes.Add(mesh);
	}

	importer.FreeScene();
	myIsValid = true;
}

Model::~Model()
{
	LOG_WARNING("Model waits for device idle. Fix");
	VulkanContext::GetDevice()->waitIdle();

	for(Mesh& mesh : myMeshes)
	{
		del(mesh.IndexBuffer);
		del(mesh.VertexBuffer);
	}
}

const List<Mesh>& Model::GetMeshes() const
{
	return myMeshes;
}

bool Model::IsValid() const
{
	return myIsValid;
}
