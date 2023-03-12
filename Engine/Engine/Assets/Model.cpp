#include "EnginePch.h"
#include "Model.h"
#include "Rendering/Vertex.hpp"
#include "Vulkan/VulkanVertexBuffer.h"
#include "Vulkan/VulkanContext.h"
#include "Vulkan/VulkanDevice.h"
#include "Vulkan/VulkanIndexBuffer.h"

Model::Model()
{
	List<Vertex> vertices
	{
		{ { -0.5f,  0.5f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }},
		{ {	 0.5f,  0.5f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }},
		{ {	 0.0f, -0.5f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }},
	};
	myVertexBuffer = new VulkanVertexBuffer(vertices);

	List<uint> indices
	{
		0, 1, 2 
	};
	myIndexBuffer = new VulkanIndexBuffer(indices);
	myIsValid = true;
}

Model::Model(const std::filesystem::path& inPath)
	: myPath{ inPath }
{
	
}

Model::~Model()
{
	LOG_WARNING("Model waits for device idle. Fix");
	VulkanContext::GetDevice()->waitIdle();

	del(myIndexBuffer);
	del(myVertexBuffer);
}

void Model::Bind(vk::CommandBuffer inCommandBuffer)
{
	myVertexBuffer->Bind(inCommandBuffer);
	myIndexBuffer->Bind(inCommandBuffer);
}

bool Model::IsValid() const
{
	return myIsValid;
}
