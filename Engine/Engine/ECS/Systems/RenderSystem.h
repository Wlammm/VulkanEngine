#pragma once

#include "../System.h"

class RenderSystem : public System<None>
{
public:
	RenderSystem();
	~RenderSystem();

	virtual void Tick() override final;

private:
	void CreatePipelines();

private:
	class VulkanPipeline* myPipeline = nullptr;
};