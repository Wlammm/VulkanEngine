#include "EnginePch.h"
#include "Model.h"
#include "Vulkan/VulkanContext.h"
#include "Vulkan/VulkanDevice.h"
#include "Engine.h"
#include "Assets/AssetRegistry.h"
#include "Assets/Material.h"
#include "Rendering/IndexBufferSystem.h"
#include "Rendering/VertexBufferSystem.h"
#include "Vulkan/VulkanAllocator.h"
#include "Vulkan/VulkanBuffer.h"

Model::Model(const List<Mesh>& inMeshes, const List<MeshHandle>& inHandles)
{
	myMeshes = inMeshes;
	myMeshHandles = inHandles;
}

Model::~Model()
{
	LOG_WARNING("Model waits for device idle. Fix");
	VulkanContext::GetDevice()->waitIdle();

	for(Mesh& mesh : myMeshes)
	{
		Engine::GetSystem<VertexBufferSystem>()->RemoveVertexBuffer(mesh.VertexBuffer);
		Engine::GetSystem<IndexBufferSystem>()->RemoveIndexBuffer(mesh.IndexBuffer);
	}
}

const List<Mesh>& Model::GetMeshes() const
{
	return myMeshes;
}
const List<MeshHandle>& Model::GetMeshHandles() const
{
	return myMeshHandles;
}

bool Model::IsValid() const
{
	return myIsValid;
}