#pragma once

class AssetRegistry
{
public:
	AssetRegistry();
	~AssetRegistry();

	class Model* GetModel(const std::filesystem::path& inPath);

private:
	std::unordered_map<std::filesystem::path, class Model*> myModels{};
};