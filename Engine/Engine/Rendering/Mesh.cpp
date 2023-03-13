#include "EnginePch.h"
#include "Mesh.h"
#include "Vulkan/VulkanVertexBuffer.h"
#include "Vulkan/VulkanIndexBuffer.h"

void Mesh::Bind(vk::CommandBuffer inCommandBuffer) const
{
	check(VertexBuffer && IndexBuffer && "Invalid buffers");
	VertexBuffer->Bind(inCommandBuffer);
	IndexBuffer->Bind(inCommandBuffer);
}
