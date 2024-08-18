#pragma once

class AssetRegistry;

class Asset
{
public:
    virtual ~Asset() = default;

    bool IsValid() const;

    virtual void Load(const std::filesystem::path& inPath) = 0;
    virtual void Unload() = 0;
    
protected:
    friend class AssetRegistry;
    AssetRegistry* GetAssetRegistry() const;

    std::filesystem::path myPath;
    bool myIsValid = false;

private:
    AssetRegistry* myAssetRegistry = nullptr;
};