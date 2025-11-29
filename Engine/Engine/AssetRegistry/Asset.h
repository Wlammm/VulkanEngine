#pragma once
#include "AssetDefines.h"

class AssetRegistry;

class Asset : std::enable_shared_from_this<Asset>
{
public:
    // Remove any resources used by this asset.
    virtual ~Asset();
    
    // This will be called once all properties on the asset have been serialized. Either via the cache or via Asset::LoadPropertiesFromSource.
    // We can remove all data not required for runtime in here as it would've already been serialized to cache before this.
    virtual void PostPropertiesSerialized() {};

    // This will get called if there is no cached version of this asset or the source asset has been updated. This will only be called if IsExternalAsset() returns true.
    virtual void LoadPropertiesFromSource() {};

    // Only checked for external assets. Internal assets are required to be backwards compatible. If this function returns false, external assets will be forced to load from source.
    virtual bool IsCacheValid() const;
    
    // If this is true we have an external source asset to track and invalidate cache when it becomes invalid. s
    // This is useful for models, textures, audio files etc, but will not be used for in engine assets such as materials, animation graph etc.
    virtual constexpr bool IsExternalAsset() const { return false; }
    
    void DoFirstTimeAssetInitialization(const SourcePath& inAssetPath);

    const SourcePath& GetSourcePath() const;

    const std::filesystem::file_time_type& GetSourceLastModifiedTime() const;

    bool IsValid() const { return myIsValid;}
    
    // Should only be called from the AssetRegistry class when its finished loading.
    void SetIsValid(const bool inIsValid);
    
    // Should only be called form the AssetRegistry class when the asset has loaded successfully.
    void SetAssetRegistry(AssetRegistry* inAssetRegistry);
    
    // This will resave the asset to cache.
    void ResaveAsset();
    
private:
    META(SerializeField)
    SourcePath myAssetPath;

    META(SerializeField)
    std::filesystem::file_time_type mySourceLastModifiedTime;

    AssetRegistry* myAssetRegistry = nullptr;
    
    bool myIsValid = false;
};
