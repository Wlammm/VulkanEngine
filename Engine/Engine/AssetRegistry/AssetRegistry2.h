#pragma once
#include "Asset2.h"
#include "AssetDefines.h"
#include "Engine/Reflection/ReflectionSystem.h"
#include "Engine/Reflection/Type.h"
#include "Engine/Serialization/BinarySerializer.h"
#include "Engine/System/System.h"

class Asset2;
class AssetRegistry2;

class AssetRegistry2 : public System
{
    inline static AssetRegistry2* mySingleton;
public:
    static AssetRegistry2* Get() { return mySingleton;}
    AssetRegistry2();
    ~AssetRegistry2() override;
    
    template<typename AssetType>
    SharedPtr<AssetType> GetAsset(const SourcePath& inSourcePath)
    {
        const Type* type = ReflectionSystem::GetType<AssetType>();
        return std::static_pointer_cast<AssetType>(GetAsset(inSourcePath, type));
        /*
        SharedPtr<AssetType> asset = std::static_pointer_cast<AssetType>(TryGetLoadedAsset(inSourcePath));
        if (asset != nullptr)
            return asset;

        if constexpr (AssetType::IsExternalAsset())
            asset = LoadExternalAsset<AssetType>(inSourcePath);
        else
            asset = LoadInternalAsset<AssetType>(inSourcePath);
        return asset;
        */
    }
    
    SharedPtr<Asset2> GetAsset(const SourcePath& inSourcePath, const Type* inType)
    {
        SharedPtr<Asset2> asset = TryGetLoadedAsset(inSourcePath);
        if (asset != nullptr)
            return asset;

        asset = inType->CreateSharedPtr<Asset2>();
        
        if (asset->IsExternalAsset())
        {
            asset = LoadExternalAsset(inSourcePath, inType);
        }
        else
        {
            asset = LoadInternalAsset(inSourcePath, inType);
        }
        
        return asset;
    }

    SharedPtr<Asset2> LoadExternalAsset(const SourcePath& inSourcePath, const Type* inType)
    {
        SharedPtr<Asset2> asset = inType->CreateSharedPtr<Asset2>();

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
        asset = inType->CreateSharedPtr<Asset2>();
        
        asset->DoFirstTimeAssetInitialization(inSourcePath);
        asset->LoadPropertiesFromSource();
        SaveAssetToCache(asset);
        
        asset->PostPropertiesSerialized();
        AddLoadedAsset(asset);
        return asset;
    }
    
    SharedPtr<Asset2> LoadInternalAsset(const SourcePath& inSourcePath, const Type* inType)
    {
        if (!std::filesystem::exists(inSourcePath))
            return nullptr;
            
        SharedPtr<Asset2> asset = inType->CreateSharedPtr<Asset2>();
        
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
        SharedPtr<AssetType> asset = std::make_shared<AssetType>();
        check(!asset->IsExternalAsset() && "We cannot create a new external asset.");
        AddLoadedAsset(asset);
        return asset;
    }

    CachePath SourceToCachePath(const SourcePath& inSourcePath) const;

    SharedPtr<Asset2> TryGetLoadedAsset(const SourcePath& inSourcePath)
    {
        std::scoped_lock lock(myMutex);

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
        serializer.SerializeType(inAsset.get(), ReflectionSystem::GetType(inAsset.get()), false);
    }
    
    void OnAssetRemoved(Asset2* inAsset);

private:
    void AddLoadedAsset(SharedPtr<Asset2> inAsset);

private:
    std::mutex myMutex{};
    std::unordered_map<SourcePath, std::weak_ptr<Asset2>> myLoadedAssets;
};
