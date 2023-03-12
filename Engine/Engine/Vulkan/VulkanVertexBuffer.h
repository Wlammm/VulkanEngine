#pragma once
#include "VulkanBuffer.h"
#include "Rendering/Vertex.hpp"


class VulkanVertexBuffer
{
public:
	VulkanVertexBuffer(const List<Vertex>& inVertices);
	~VulkanVertexBuffer();

	void Bind(vk::CommandBuffer inCommandBuffer);

private:
	VulkanBuffer* myBuffer = nullptr;
	
};