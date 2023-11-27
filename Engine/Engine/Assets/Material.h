#pragma once
#include "AssetObserver.h"

class VulkanPipeline;
class VulkanShader;

class Material : public AssetObserver
{
public:
	Material();
	~Material();

	void Create();
	void Destroy();

	void Bind(vk::CommandBuffer inCommandBuffer) const;

private:
	VulkanPipeline* myPipeline;
	VulkanShader* myVertexShader;
	VulkanShader* myPixelShader;

	class VulkanTexture* myTexture = nullptr;


	// Inherited via AssetObserver
	virtual void OnAssetUpdated() override;

	/*
	* VertexShader
	* FragmentShader
	* Albedo
	* */
};