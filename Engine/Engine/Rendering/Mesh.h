#pragma once

struct Mesh
{
	uint NumVertices = 0;
	uint NumIndices = 0;

	class VulkanVertexBuffer* VertexBuffer = nullptr;
	class VulkanIndexBuffer* IndexBuffer = nullptr;

	class Material* myMaterial = nullptr;

	void Bind(vk::CommandBuffer inCommandBuffer) const;
};