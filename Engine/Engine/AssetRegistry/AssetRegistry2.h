#pragma once
#include "Engine/Serialization/BinarySerializer.h"
#include "Engine/System/System.h"

class AssetRegistry2;


// Used to indicate paths to the source file. Say .fbx, .png, etc
using SourcePath = std::filesystem::path;

// Used to indicate to a path inside the cache.
using CachePath = std::filesystem::path;

class Asset2
{
public:
    ~Asset2();
    
    // This will be called once all properties on the asset have been serialized. Either via the cache or via Asset::LoadPropertiesFromSource.
    // We can remove all data not required for runtime in here as it would've already been serialized to cache before this.
    virtual void PostPropertiesSerialized() {};

    // This will get called if there is no cached version of this asset or the source asset has been updated.
    virtual void LoadPropertiesFromSource() {};

    // Remove any resources used by this asset.
    virtual void Unload() {};

    void DoFirstTimeAssetInitialization(const SourcePath& inAssetPath);

    const SourcePath& GetSourcePath();

    const std::filesystem::file_time_type& GetSourceLastModifiedTime() const;

    // Should only be called from the AssetRegistry class when its finished loading.
    void SetIsValid(const bool inIsValid);
    
    // Should only be called form the AssetRegistry class when the asset has loaded successfully.
    void SetAssetRegistry(AssetRegistry2* inAssetRegistry);
    
private:
    META(SerializeField)
    SourcePath myAssetPath;

    META(SerializeField)
    std::filesystem::file_time_type mySourceLastModifiedTime;

    AssetRegistry2* myAssetRegistry = nullptr;
    
    bool myIsValid = false;
};

class AssetRegistry2 : public System
{
public:
    template<typename AssetType>
    SharedPtr<AssetType> GetAsset(const SourcePath& inSourcePath)
    {
        SharedPtr<AssetType> asset = std::static_pointer_cast<AssetType>(TryGetLoadedAsset(inSourcePath));
        if (asset != nullptr)
            return asset;

        asset = LoadAsset<AssetType>(inSourcePath);
        return asset;
    }

    template<typename AssetType>
    SharedPtr<AssetType> LoadAsset(const SourcePath& inSourcePath)
    {
        SharedPtr<AssetType> asset = std::make_shared<AssetType>();

        const CachePath cachePath = SourceToCachePath(inSourcePath);

        if (std::filesystem::exists(cachePath))
        {
            bool isCachedFileValid = true;
            
            // Try load cached version.
            BinarySerializer serializer(cachePath, BinarySerializer::Mode::Read);
            serializer.SerializeType(*asset.get());
            if (!serializer.WasLastTypeSerializationFullyComplete())
                isCachedFileValid = false;
            serializer.Close();

            std::filesystem::file_time_type sourceWriteTime = std::filesystem::last_write_time(inSourcePath);

            // Check if source has been modified since the cache was last written.
            if (asset->GetSourceLastModifiedTime() != sourceWriteTime)
                isCachedFileValid = false;

            if (isCachedFileValid)
            {
                asset->PostPropertiesSerialized();
                AddLoadedAsset(asset);
                return asset;
            }
        }

        // Cache is not valid. Reset the asset and initialize it from source.
        asset = std::make_shared<AssetType>();
        
        asset->DoFirstTimeAssetInitialization(inSourcePath);
        asset->LoadPropertiesFromSource();
        SaveAssetToCache(asset);
        
        asset->PostPropertiesSerialized();
        AddLoadedAsset(asset);
        return asset;
    }

    CachePath SourceToCachePath(const SourcePath& inSourcePath) const;

    SharedPtr<Asset2> TryGetLoadedAsset(const SourcePath& inSourcePath)
    {
        std::scoped_lock lock(myMutex);
        LOG("Locked in TryGetLoadedAsset");

        if (myLoadedAssets.find(inSourcePath) != myLoadedAssets.end())
        {
            SharedPtr<Asset2> asset = myLoadedAssets.at(inSourcePath).lock();
            check(asset != nullptr);
            return asset;
        }
        return nullptr;
    }

    void OnAssetRemoved(Asset2* inAsset);

private:
    void AddLoadedAsset(SharedPtr<Asset2> inAsset);

    template<typename AssetType>
    void SaveAssetToCache(SharedPtr<AssetType> inAsset)
    {
        CachePath cachePath = SourceToCachePath(inAsset->GetSourcePath());
        BinarySerializer serializer(cachePath, BinarySerializer::Mode::Write);
        serializer.SerializeType(*inAsset.get());
    }

private:
    std::mutex myMutex{};
    std::unordered_map<SourcePath, std::weak_ptr<Asset2>> myLoadedAssets;
};
