#pragma once
#include "EditorWindow.h"
#include "Engine/Events/EventObserver.h"

class Viewport : public EditorWindow, public EventObserver
{
public:
	Viewport();
	~Viewport();

	virtual void Tick() override final;

private:
	//void OnSwapchainResized();

private:
	//void DestroyDescriptorSet();
	//void CreateDescriptorSet();

	vk::DescriptorSet GetCurrentDescriptorSet();
	void UpdateCurrentTexture();

private:
	List<vk::DescriptorSet> myDescriptorSets;
	vk::Sampler mySampler;
};