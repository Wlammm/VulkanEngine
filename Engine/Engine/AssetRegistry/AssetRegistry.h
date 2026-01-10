#pragma once
#include "Asset.h"
#include "AssetDefines.h"
#include "Engine/Engine.h"
#include "Engine/Delegates/MulticastDelegate.hpp"
#include "Engine/Reflection/ReflectionSystem.h"
#include "Engine/Reflection/Type.h"
#include "Engine/Serialization/BinarySerializer.h"
#include "Engine/System/System.h"
#include "Engine/Utils/ThreadUtils.hpp"

class Asset;
class AssetRegistry;

class AssetRegistry : public System
{
    inline static AssetRegistry* mySingleton;
    using OnAsyncLoadCompleteDelegate = MulticastDelegate<void(SharedPtr<Asset> inOnComplete)>;
    
public:
    static AssetRegistry* Get() { return mySingleton; }
    
    AssetRegistry();
    ~AssetRegistry() override;
    
    /*
     * inOnCompleteDelegate will always get called on the game thread. 
     */
    template<typename AssetType>
    void GetAssetAsync(const SourcePath& inSourcePath, const Delegate<void(SharedPtr<AssetType> inAsset)>& inOnCompleteDelegate)
    {
        const Type* type = ReflectionSystem::GetType<AssetType>();
        
        // We need to do this as Delegate<Asset> and Delegate<DerivedAssetType> is not convertible.
        Delegate<void(SharedPtr<Asset>)> wrapper = [inOnCompleteDelegate](SharedPtr<Asset> baseAsset)
        {
            SharedPtr<AssetType> derived = std::static_pointer_cast<AssetType>(baseAsset);
            inOnCompleteDelegate(derived);
        };
        
        GetAssetAsync(inSourcePath, type, wrapper);
    }
    void GetAssetAsync(const SourcePath& inSourcePath, const Type* inType, const Delegate<void(SharedPtr<Asset> inAsset)>& inOnCompleteDelegate);
    
    template<typename AssetType>
    SharedPtr<AssetType> GetAssetSynchronous(const SourcePath& inSourcePath)
    {
        const Type* type = ReflectionSystem::GetType<AssetType>();
        return std::static_pointer_cast<AssetType>(GetAssetSynchronous(inSourcePath, type));
    }
    
    // TODO: maybe we should only return const Asset's here as we dont want anything changing them. 
    // If you want to change it you have to make your own instance of it. This instance can then be saved back in explicitly.
    SharedPtr<Asset> GetAssetSynchronous(const SourcePath& inSourcePath, const Type* inType);

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
                AddLoadedAsset(asset, inType);
                return asset;
            }
        }

        // Cache is not valid. Reset the asset and initialize it from source.
        asset = inType->CreateSharedPtr<Asset>();
        
        asset->DoFirstTimeAssetInitialization(inSourcePath);
        asset->LoadPropertiesFromSource();
        asset->SetType(inType);
        SaveAsset(asset);
        
        if (ThreadUtils::IsOnMainThread())
        {
            asset->PostPropertiesSerialized();
        }
        else
        {
            Engine::TickNextFrame.Bind([asset]()
            {
                asset->PostPropertiesSerialized();
            });     
        }
        AddLoadedAsset(asset, inType);
        return asset;
    }
    
    SharedPtr<Asset> LoadInternalAsset(const SourcePath& inSourcePath, const Type* inType)
    {
        if (!std::filesystem::exists(inSourcePath))
            return nullptr;
            
        SharedPtr<Asset> asset = inType->CreateSharedPtr<Asset>();
        
        BinarySerializer serializer(inSourcePath, BinarySerializer::Mode::Read);
        // Sleep(100);
        serializer.SerializeType(asset.get(), ReflectionSystem::GetType(asset.get()), false);

        if (!serializer.WasLastTypeSerializationFullyComplete())
            LOG_WARNING("LoadInternalAsset: Not all properties was loaded correctly on %s", inSourcePath.string().c_str());
            
        serializer.Close();
        
        if (ThreadUtils::IsOnMainThread())
        {
            asset->PostPropertiesSerialized();
        }
        else
        {
            Engine::TickNextFrame.Bind([asset]()
            {
                asset->PostPropertiesSerialized();
            });
        }
        AddLoadedAsset(asset, inType);
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
        AddLoadedAsset(asset, inType);
        return asset;
    }
    
    // This will redirect the path to another location for some asset types. Used for Shaders as an example. Anything that starts with Shaders/ will be redirected to ../Engine/Engine/Shaders/
    SourcePath TryRedirectPath(const SourcePath& inSourcePath) const;

    CachePath SourceToCachePath(const SourcePath& inSourcePath) const;

    SharedPtr<Asset> TryGetLoadedAsset(const SourcePath& inSourcePath)
    {
        ZoneScoped;
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
        ZoneScoped;
        CachePath savePath;
        if (inAsset->GetType()->CallStaticMethodRecursive<bool>("IsExternalAsset"))
            savePath = SourceToCachePath(inAsset->GetSourcePath());
        else
            savePath = inAsset->GetSourcePath();
        
        BinarySerializer serializer(savePath, BinarySerializer::Mode::Write);
        serializer.SerializeType(inAsset.get(), ReflectionSystem::GetType(inAsset.get()), false);
    }
    
    void OnAssetRemoved(Asset* inAsset);
    
    
    template<typename AssetType>
    SharedPtr<AssetType> CreateAssetInstance(const SourcePath& inSourcePath)
    {
        const Type* type = ReflectionSystem::GetType<AssetType>();
        
        SharedPtr<AssetType> asset = type->CreateSharedPtr<AssetType>();
        
        SharedPtr<Asset> existingAsset = GetAssetSynchronous(inSourcePath, type);
        if (existingAsset)
        {
            asset->SetClassDefaultAsset(existingAsset);
            ReflectionSystem::CopyProperties(existingAsset.get(), asset.get(), type);
        }
        else
        {
            check(false && "failed to create asset instance. Could not get type");
        }
        
        asset->DoFirstTimeAssetInitialization(inSourcePath);
        asset->PostPropertiesSerialized();
        asset->SetIsValid(true);
        return asset;
    }
    
private:
    void AddLoadedAsset(SharedPtr<Asset> inAsset, const Type* inType);
    
    // If the sent in asset is currently pending we will wait for it to be done.
    SharedPtr<Asset> WaitForPendingAsset(const SourcePath& inSourcePath);
    
private:
    mutable std::recursive_mutex myMutex{};
    std::unordered_map<SourcePath, std::weak_ptr<Asset>> myLoadedAssets;

    // Assets that is currently being asynchronously loaded.
    MutexList<SourcePath> myPendingAssets;
};
