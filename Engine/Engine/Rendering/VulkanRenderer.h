#pragma once

class VulkanRenderer
{
public:
	VulkanRenderer();
	~VulkanRenderer();

	void Tick();

private:
	void CreatePipelines();

private:
	class VulkanPipeline* myPipeline = nullptr;

};