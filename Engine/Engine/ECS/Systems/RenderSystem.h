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

private:
	class VulkanPipeline* myPipeline = nullptr;


	struct FrameBuffer
	{
		Mat4f myToView;
		Mat4f myProjection;
	};

	VulkanUniformBuffer<FrameBuffer> myFrameBuffer;
};