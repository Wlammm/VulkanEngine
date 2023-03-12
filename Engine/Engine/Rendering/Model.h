#pragma once

class Model
{
public:
	Model();
	~Model();

	void Bind(vk::CommandBuffer inCommandBuffer);

private:
	class VulkanVertexBuffer* myVertexBuffer;

};