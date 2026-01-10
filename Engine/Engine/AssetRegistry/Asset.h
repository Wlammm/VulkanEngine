#pragma once
#include "AssetDefines.h"

class AssetRegistry;

/*
 * IMPORTANT: Make sure the asset always overrides the static method GetAssetExtensions() and optionally IsExternalAsset if it is.
 */
class Asset : std::enable_shared_from_this<Asset>
{
public:
    // Remove any resources used by this asset.
    virtual ~Asset();
    
    // All assets need to implement their own version of this and add a list of all accepted extensions for a given asset. Example: { ".fbx", ".gltf" } or { ".mat" }
    // for internal assets you should ignore the final .cache extension.
    static List<std::string> GetAssetExtensions() { check(false && "All assets need to implement their own static version of this function."); return {}; }

    // If this is true we have an external source asset to track and invalidate cache when it becomes invalid. s
    // This is useful for models, textures, audio files etc, but will not be used for in engine assets such as materials, animation graph etc.
    static constexpr bool IsExternalAsset() { return false; }
    
    
    // This will be called once all properties on the asset have been serialized. Either via the cache or via Asset::LoadPropertiesFromSource.
    // We can remove all data not required for runtime in here as it would've already been serialized to cache before this.
    virtual void PostPropertiesSerialized() {};
    
    // This will get called if there is no cached version of this asset or the source asset has been updated. This will only be called if IsExternalAsset() returns true.
    virtual void LoadPropertiesFromSource() {};

    // Only checked for external assets. Internal assets are required to be backwards compatible. If this function returns false, external assets will be forced to load from source.
    virtual bool IsCacheValid() const;
    
    void DoFirstTimeAssetInitialization(const SourcePath& inAssetPath);
    
    // Changes the internal assets path. This shouldnt be used by itself. Use AssetUtils::Rename function if you want to move the asset.
    void SetAssetPath(const SourcePath& inSourcePath);

    const SourcePath& GetSourcePath() const;
    
    std::string GetName() const;

    const std::filesystem::file_time_type& GetSourceLastModifiedTime() const;

    bool IsValid() const { return myIsValid;}
    
    // Should only be called from the AssetRegistry class when its finished loading.
    void SetIsValid(const bool inIsValid);
    
    // Should only be called form the AssetRegistry class when the asset has loaded successfully.
    void SetAssetRegistry(AssetRegistry* inAssetRegistry);
    
    // Should only be called from the AssetRegistry when the asset is being loaded.
    void SetType(const Type* inType);
    
    const Type* GetType() const;
    
    // This will resave the asset to cache.
    void ResaveAsset();
    
    void SetClassDefaultAsset(SharedPtr<Asset> inAsset);
    
private:
    META(SerializeField, HideInEditor)
    SourcePath myAssetPath;

    META(SerializeField, HideInEditor)
    std::filesystem::file_time_type mySourceLastModifiedTime;

    AssetRegistry* myAssetRegistry = nullptr;
    const Type* myType = nullptr;
    
    // This is used to keep the base asset alive for instances
    SharedPtr<Asset> myClassDefaultAsset = nullptr;
    
    bool myIsValid = false;
};
