#pragma once
#include "Asset.h"
#include "AssetDefines.h"
#include "Engine/Reflection/ReflectionSystem.h"
#include "Engine/Reflection/Type.h"
#include "Engine/Serialization/BinarySerializer.h"
#include "Engine/System/System.h"

class Asset;
class AssetRegistry;

class AssetRegistry : public System
{
    inline static AssetRegistry* mySingleton;
public:
    static AssetRegistry* Get() { return mySingleton; }
    
    AssetRegistry();
    ~AssetRegistry() override;
    
    template<typename AssetType>
    SharedPtr<AssetType> GetAsset(const SourcePath& inSourcePath)
    {
        const Type* type = ReflectionSystem::GetType<AssetType>();
        return std::static_pointer_cast<AssetType>(GetAsset(inSourcePath, type));
    }
    
    SharedPtr<Asset> GetAsset(const SourcePath& inSourcePath, const Type* inType)
    {
        SharedPtr<Asset> asset = TryGetLoadedAsset(inSourcePath);
        if (asset != nullptr)
            return asset;

        asset = inType->CreateSharedPtr<Asset>();
        
        if (inType->CallStaticMethodRecursive<bool>("IsExternalAsset"))
        {
            asset = LoadExternalAsset(inSourcePath, inType);
        }
        else
        {
            asset = LoadInternalAsset(inSourcePath, inType);
        }
        
        return asset;
    }

    SharedPtr<Asset> LoadExternalAsset(const SourcePath& inSourcePath, const Type* inType)
    {
        SharedPtr<Asset> asset = inType->CreateSharedPtr<Asset>();

        const CachePath cachePath = SourceToCachePath(inSourcePath);
        
        if (std::filesystem::exists(cachePath))
        {
            bool isCachedFileValid = true;
            
            // Try load cached version.
            BinarySerializer serializer(cachePath, BinarySerializer::Mode::Read);
            serializer.SerializeType(asset.get(), ReflectionSystem::GetType(asset.get()), false);
            if (!serializer.WasLastTypeSerializationFullyComplete())
                isCachedFileValid = false;
            serializer.Close();
            
            if (!asset->IsCacheValid())
                isCachedFileValid = false;

            if (isCachedFileValid)
            {
                asset->PostPropertiesSerialized();
                AddLoadedAsset(asset);
                return asset;
            }
        }

        // Cache is not valid. Reset the asset and initialize it from source.
        asset = inType->CreateSharedPtr<Asset>();
        
        asset->DoFirstTimeAssetInitialization(inSourcePath);
        asset->LoadPropertiesFromSource();
        SaveAsset(asset);
        
        asset->PostPropertiesSerialized();
        AddLoadedAsset(asset);
        return asset;
    }
    
    SharedPtr<Asset> LoadInternalAsset(const SourcePath& inSourcePath, const Type* inType)
    {
        if (!std::filesystem::exists(inSourcePath))
            return nullptr;
            
        SharedPtr<Asset> asset = inType->CreateSharedPtr<Asset>();
        
        BinarySerializer serializer(inSourcePath, BinarySerializer::Mode::Read);
        serializer.SerializeType(asset.get(), ReflectionSystem::GetType(asset.get()), false);

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
        return std::static_pointer_cast<AssetType>(CreateNewAsset(inSourcePath, ReflectionSystem::GetType<AssetType>()));
    }
    
    SharedPtr<Asset> CreateNewAsset(const SourcePath& inSourcePath, const Type* inType)
    {
        SharedPtr<Asset> asset = inType->CreateSharedPtr<Asset>();
        check(!inType->CallStaticMethodRecursive<bool>("IsExternalAsset") && "We cannot create a new external asset.");
        check(inType->CallStaticMethodRecursive<List<std::string>>("GetAssetExtensions").Contains(inSourcePath.extension().string()) && "New assets must follow the allowed extensions.");
        
        asset->DoFirstTimeAssetInitialization(inSourcePath);
        AddLoadedAsset(asset);
        return asset;
    }

    CachePath SourceToCachePath(const SourcePath& inSourcePath) const;

    SharedPtr<Asset> TryGetLoadedAsset(const SourcePath& inSourcePath)
    {
        std::scoped_lock lock(myMutex);

        if (myLoadedAssets.find(inSourcePath) != myLoadedAssets.end())
        {
            SharedPtr<Asset> asset = myLoadedAssets.at(inSourcePath).lock();
            check(asset != nullptr);
            return asset;
        }
        return nullptr;
    }

    template<typename AssetType>
    void SaveAsset(SharedPtr<AssetType> inAsset)
    {
        CachePath savePath;
        if (inAsset->IsExternalAsset())
            savePath = SourceToCachePath(inAsset->GetSourcePath());
        else
            savePath = inAsset->GetSourcePath();
        
        BinarySerializer serializer(savePath, BinarySerializer::Mode::Write);
        serializer.SerializeType(inAsset.get(), ReflectionSystem::GetType(inAsset.get()), false);
    }
    
    void OnAssetRemoved(Asset* inAsset);
    
private:
    void AddLoadedAsset(SharedPtr<Asset> inAsset);

private:
    std::mutex myMutex{};
    std::unordered_map<SourcePath, std::weak_ptr<Asset>> myLoadedAssets;
};
