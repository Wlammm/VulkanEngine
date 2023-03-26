#pragma once
#include "EditorWindow.h"

class Viewport : public EditorWindow
{
public:
	Viewport();

	virtual void Tick() override final;


private:
	void CreateDescriptorSets();
	vk::DescriptorSet GetCurrentDescriptorSet();

private:
	List<vk::DescriptorSet> myDescriptorSets;
	vk::Sampler mySampler;
};