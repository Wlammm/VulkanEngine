#include "EnginePch.h"
#include "Mesh.h"
#include "Vulkan/VulkanBuffer.h"

void Mesh::Bind(vk::CommandBuffer inCommandBuffer) const
{
	check(VertexBuffer && IndexBuffer && "Invalid buffers");
	inCommandBuffer.bindVertexBuffers(0, { VertexBuffer->GetAPIResource() }, { 0 });
	inCommandBuffer.bindIndexBuffer(IndexBuffer->GetAPIResource(), 0, vk::IndexType::eUint32);
}
