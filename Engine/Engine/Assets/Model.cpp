#include "EnginePch.h"
#include "Model.h"
#include "Vulkan/VulkanContext.h"
#include "Vulkan/VulkanDevice.h"
#include "Engine.h"
#include "Assets/AssetRegistry.h"
#include "Assets/Material.h"
#include "Vulkan/VulkanAllocator.h"
#include "Vulkan/VulkanBuffer.h"

Model::Model(const List<Mesh>& inMeshes)
{
	myMeshes = inMeshes;
}

Model::~Model()
{
	LOG_WARNING("Model waits for device idle. Fix");
	VulkanContext::GetDevice()->waitIdle();

	for(Mesh& mesh : myMeshes)
	{
		VulkanAllocator::DestroyBuffer_TS(mesh.IndexBuffer);
		VulkanAllocator::DestroyBuffer_TS(mesh.VertexBuffer);
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