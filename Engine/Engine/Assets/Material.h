#pragma once
#include "Engine/AssetRegistry/Asset.h"

class Texture;

class Material : public Asset
{
public:
	virtual constexpr bool IsExternalAsset() const override { return false; }
	
	List<std::string> GetAssetExtensions() const override { return { ".mat" }; }
	
	SharedPtr<Texture> GetAlbedo() const;
	SharedPtr<Texture> GetNormal() const;
	SharedPtr<Texture> GetMaterial() const;

	void SetAlbedo(SharedPtr<Texture> inTexture);
	void SetAlbedo(const SourcePath& inSourcePath);
	
	void SetNormal(SharedPtr<Texture> inTexture);
	void SetNormal(const SourcePath& inSourcePath);
	
	void SetMaterial(SharedPtr<Texture> inTexture);
	void SetMaterial(const SourcePath& inSourcePath);
	
	bool GetDepthWriteEnabled() const;

private:
	META(SerializeField)
	SharedPtr<Texture> myAlbedoTexture = nullptr;
	
	META(SerializeField)
	SharedPtr<Texture> myNormalTexture = nullptr;
	
	META(SerializeField)
	SharedPtr<Texture> myMaterialTexture = nullptr;

	META(SerializeField)
	bool myDepthWriteEnabled = true;
};