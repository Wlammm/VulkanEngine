#pragma once
#include "Engine/Serialization/BinarySerializer.h"
#include "Engine/System/System.h"

class AssetRegistry2;


/*
 * Internal asset: An asset that is made in engine and modified in engine. No external files are managed for this asset type.
 * 
 * External asset: These are assets that are imported to the engine. Most of the time they are converted to a binary format the 
 * engine can read faster but it also stores the last modification time so if the asset gets modified outside the engine, we will automatically reimport it next time.
 */

// This is the path to the internal asset on disk, or cached version of external asset.
using CachePath = std::filesystem::path;

// This would be the path to the external asset if it is such, otherwise it is just a unique identified to the internal asset, usually just the name of the asset.
using SourcePath = std::filesystem::path;

class Asset2 : std::enable_shared_from_this<Asset2>
{
public:
    // Remove any resources used by this asset.
    virtual ~Asset2();
    
    // This will be called once all properties on the asset have been serialized. Either via the cache or via Asset::LoadPropertiesFromSource.
    // We can remove all data not required for runtime in here as it would've already been serialized to cache before this.
    virtual void PostPropertiesSerialized() {};

    // This will get called if there is no cached version of this asset or the source asset has been updated. This will only be called if IsExternalAsset() returns true.
    virtual void LoadPropertiesFromSource() {};

    // Override this and return false if this asset should always load from source.
    static constexpr bool CanAssetBeCached() { return true; }
    
    // If this is true we have an external source asset to track and invalidate cache when it becomes invalid. 
    // This is useful for models, textures, audio files etc, but will not be used for in engine assets such as materials, animation graph etc.
    static constexpr bool IsExternalAsset() { return false; }
    
    void DoFirstTimeAssetInitialization(const SourcePath& inAssetPath);

    const SourcePath& GetSourcePath();

    const std::filesystem::file_time_type& GetSourceLastModifiedTime() const;

    bool IsValid() const { return myIsValid;}
    
    // Should only be called from the AssetRegistry class when its finished loading.
    void SetIsValid(const bool inIsValid);
    
    // Should only be called form the AssetRegistry class when the asset has loaded successfully.
    void SetAssetRegistry(AssetRegistry2* inAssetRegistry);
    
    // This will resave the asset to file.
    void ResaveAsset();
    
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

        if constexpr (AssetType::IsExternalAsset())
            asset = LoadExternalAsset<AssetType>(inSourcePath);
        else
            asset = LoadInternalAsset<AssetType>(inSourcePath);
        return asset;
    }

    template<typename AssetType>
    SharedPtr<AssetType> LoadExternalAsset(const SourcePath& inSourcePath)
    {
        SharedPtr<AssetType> asset = std::make_shared<AssetType>();

        const CachePath cachePath = SourceToCachePath(inSourcePath);
        
        if (std::filesystem::exists(cachePath) && AssetType::CanAssetBeCached())
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
    
    template<typename AssetType>
    SharedPtr<AssetType> LoadInternalAsset(const SourcePath& inSourcePath)
    {
        if (!std::filesystem::exists(inSourcePath))
            return nullptr;
            
        SharedPtr<AssetType> asset = std::make_shared<AssetType>();
        
        BinarySerializer serializer(inSourcePath, BinarySerializer::Mode::Read);
        serializer.SerializeType(*asset.get());

        if (!serializer.WasLastTypeSerializationFullyComplete())
            LOG_WARNING("LoadInternalAsset: Not all properties was loaded correctly on %s", inSourcePath.string().c_str());
            
        serializer.Close();
        
        asset->PostPropertiesSerialized();
        AddLoadedAsset(asset);
        return asset;
    }
    
    template<typename AssetType>
    SharedPtr<AssetType> CreateNewAsset(const SourcePath& inSourcePath)
    {
        static_assert(!AssetType::IsExternalAsset() && "We cannot create new external assets.");
        SharedPtr<AssetType> asset = std::make_shared<AssetType>();
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

    template<typename AssetType>
    void SaveAssetToCache(SharedPtr<AssetType> inAsset)
    {
        CachePath cachePath = SourceToCachePath(inAsset->GetSourcePath());
        BinarySerializer serializer(cachePath, BinarySerializer::Mode::Write);
        serializer.SerializeType(*inAsset.get());
    }
    
    void OnAssetRemoved(Asset2* inAsset);

private:
    void AddLoadedAsset(SharedPtr<Asset2> inAsset);

private:
    std::mutex myMutex{};
    std::unordered_map<SourcePath, std::weak_ptr<Asset2>> myLoadedAssets;
};
