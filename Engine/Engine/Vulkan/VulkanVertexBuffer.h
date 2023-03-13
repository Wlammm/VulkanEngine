#pragma once
#include "Rendering/Vertex.hpp"

class VulkanVertexBuffer
{
public:
	VulkanVertexBuffer() = delete;
	VulkanVertexBuffer(const List<Vertex>& inVertices);
	~VulkanVertexBuffer();

	void Bind(vk::CommandBuffer inCommandBuffer) const;

private:
	class VulkanBuffer* myBuffer = nullptr;
	
};