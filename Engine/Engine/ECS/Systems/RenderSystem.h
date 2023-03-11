#pragma once

#include "../System.h"
#include "Vulkan/VulkanUniformBuffer.hpp"
#include "Math/Camera.h"
#include "Math/Transform.h"

class RenderSystem : public System<const Transform, const Camera>
{
public:
	RenderSystem();
	~RenderSystem();

	virtual void Tick() override final;

private:
	void CreatePipelines();

	void UpdateFrameBuffer();
	void UpdateObjectBuffer(const Transform& inTransform);

private:
	class VulkanPipeline* myPipeline = nullptr;

	struct FrameBuffer
	{
		Mat4f myToView;
		Mat4f myProjection;
	};
	VulkanUniformBuffer<FrameBuffer> myFrameBuffer{vk::ShaderStageFlagBits::eVertex, 0 };

	struct ObjectBuffer
	{
		Mat4f myToWorld;
	};
	VulkanUniformBuffer<ObjectBuffer> myObjectBuffer{ vk::ShaderStageFlagBits::eVertex, 1 };
};