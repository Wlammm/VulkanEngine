#pragma once
#include "VertexBufferSystem.h"

struct Mesh
{
	uint NumVertices = 0;
	uint NumIndices = 0;

	VertexBufferHandle VertexBuffer;
	class VulkanBuffer* IndexBuffer = nullptr;

	class Material* myMaterial = nullptr;

	void Bind(vk::CommandBuffer inCommandBuffer) const;
};
