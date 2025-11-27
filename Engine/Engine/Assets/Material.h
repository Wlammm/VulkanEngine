#pragma once
#include "Engine/AssetRegistry/AssetRegistry2.h"

class Texture;

class Material : public Asset2
{
public:
	void PostPropertiesSerialized() override;
	
	static constexpr bool IsExternalAsset() { return false; }
	
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
	SourcePath myAlbedoPath;
	
	META(SerializeField)
	SourcePath myMaterialPath;
	
	META(SerializeField)
	SourcePath myNormalPath;
	
	SharedPtr<Texture> myAlbedoTexture = nullptr;
	SharedPtr<Texture> myNormalTexture = nullptr;
	SharedPtr<Texture> myMaterialTexture = nullptr;

	META(SerializeField)
	bool myDepthWriteEnabled = true;
};