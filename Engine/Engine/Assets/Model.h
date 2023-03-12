#pragma once

class Model
{
public:
	void Bind(vk::CommandBuffer inCommandBuffer);

	bool IsValid() const;

private:
	// ------- TEMP -------
	friend class RenderSystem;

	// Only asset registry should create instances of this class.
	friend class AssetRegistry;
	Model();
	Model(const std::filesystem::path& inPath);
	~Model();

private:
	bool myIsValid = false;
	std::filesystem::path myPath = "";

	class VulkanVertexBuffer* myVertexBuffer = nullptr;
	class VulkanIndexBuffer* myIndexBuffer = nullptr;
};