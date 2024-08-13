#include "EnginePch.h"
#include "Model.h"

#include "Engine.h"
#include "Vulkan/VulkanContext.h"
#include "Vulkan/VulkanDevice.h"

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
		Engine::GetEngineSystem<VertexBufferSystem>().RemoveVertexBuffer(mesh.VertexBuffer);
		Engine::GetEngineSystem<IndexBufferSystem>().RemoveIndexBuffer(mesh.IndexBuffer);
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