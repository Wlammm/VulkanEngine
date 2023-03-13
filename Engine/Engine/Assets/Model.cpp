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

Model::Model(const std::filesystem::path& inPath)
	: myPath{ inPath }
{
	Assimp::Importer importer;
	
	uint flags = aiProcessPreset_TargetRealtime_MaxQuality |
		aiProcess_ConvertToLeftHanded |
		aiProcess_Triangulate |
		aiProcess_CalcTangentSpace |
		aiProcess_SortByPType;
	flags &= ~aiProcess_JoinIdenticalVertices;

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

		mesh.NumVertices = aiMesh->mNumVertices;
		mesh.NumIndices = aiMesh->mNumFaces * 3;

		for (uint vertexIndex = 0; vertexIndex < mesh.NumVertices; ++vertexIndex)
		{
			Vertex vertex{};
			vertex.myPosition = { aiMesh->mVertices[vertexIndex].x, aiMesh->mVertices[vertexIndex].y, aiMesh->mVertices[vertexIndex].z, 1.0f };
			vertex.myColor = { 1, 1, 1, 1 };

			if (aiMesh->GetNumColorChannels() > 0)
				vertex.myColor = { aiMesh->mColors[vertexIndex][0].r, aiMesh->mColors[vertexIndex][0].g, aiMesh->mColors[vertexIndex][0].b, aiMesh->mColors[vertexIndex][0].a };

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

		mesh.VertexBuffer = new VulkanVertexBuffer(vertices);
		mesh.IndexBuffer = new VulkanIndexBuffer(indices);
		myMeshes.Add(mesh);
	}
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
