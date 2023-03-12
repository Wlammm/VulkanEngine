#include "EnginePch.h"
#include "Model.h"
#include "Vertex.hpp"
#include "Vulkan/VulkanVertexBuffer.h"
#include "Vulkan/VulkanContext.h"
#include "Vulkan/VulkanDevice.h"

Model::Model()
{
	List<Vertex> vertices
	{
		{ { -0.5f,  0.5f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }},
		{ {	 0.5f,  0.5f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }},
		{ {	 0.0f, -0.5f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }},
	};

	myVertexBuffer = new VulkanVertexBuffer(vertices);
}

Model::~Model()
{
	LOG_WARNING("Model waits for device idle. Fix");
	VulkanContext::GetDevice()->waitIdle();

	del(myVertexBuffer);
}

void Model::Bind(vk::CommandBuffer inCommandBuffer)
{
	myVertexBuffer->Bind(inCommandBuffer);
}
