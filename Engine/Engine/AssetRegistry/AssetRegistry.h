#pragma once
#include "Asset.h"
#include "AssetContainer.h"
#include "Engine.h"
#include "Core/ThreadPool.h"
#include "Coroutines/Coroutine.h"
#include "Delegates/Delegate.hpp"
#include "Utils/ThreadUtils.hpp"

class Asset;

class AssetRegistry
{
public:
    AssetRegistry();
    ~AssetRegistry();

    static void ScanAssetsFolder();
    static std::filesystem::path GetPathFromAssetName(const std::string& inAssetName);

    // This method can be used to register temporary generated assets. Example of this can be a generated mesh material based on the texture paths
    // extracted from the mesh. The inAsset argument should be a heap allocated asset and the lifetime ownership will be transferred to this AssetRegistry
    // which will handle the deletion of the asset.
    template<typename AssetType>
    void RegisterTemporaryAsset(const std::filesystem::path& inAssetPath, AssetType* inAsset)
    {
        std::scoped_lock lock(myMutex);
        AssetContainer<AssetType>* container = GetContainerForAssetType<AssetType>();
        
        inAsset->myAssetRegistry = this;
        if(container->myAssets.contains(inAssetPath))
        {
            // TODO: Delete the inAsset here?
            return;
        }

        check(inAsset);
        container->myAssets.insert({inAssetPath, inAsset});
    }

    template<typename AssetType>
    bool HasAsset(const std::filesystem::path& inAssetPath)
    {
        std::scoped_lock lock(myMutex);
        AssetContainer<AssetType>* container = GetContainerForAssetType<AssetType>();
        return container->myAssets.contains(inAssetPath);
    }

    template <typename AssetType>
    void GetAssetAsync(const std::filesystem::path& inPath, const Delegate<void(AssetType* inAsset)> inOnAssetLoaded)
    {
        ZoneScoped;
        std::scoped_lock lock(myMutex);
        AssetContainer<AssetType>* container = GetContainerForAssetType<AssetType>();
        
        if (container->myPendingOnAssetLoaded.contains(inPath))
        {
            AssetType* asset = static_cast<AssetType*>(container->myAssets.at(inPath));
            container->myPendingOnAssetLoaded[inPath].Bind([inOnAssetLoaded, asset]()
            {
                inOnAssetLoaded(asset);
            });
            return;
        }

        if(container->myAssets.contains(inPath))
        {
            inOnAssetLoaded(static_cast<AssetType*>(container->myAssets.at(inPath)));
            return;
        }

        AssetType* asset = new AssetType();
        asset->myPath = inPath;
        asset->myAssetRegistry = this;
        container->myAssets.insert({inPath, asset});
        
        container->myPendingOnAssetLoaded.insert({inPath, {}});
        container->myPendingOnAssetLoaded[inPath].Bind([inOnAssetLoaded, asset]()
        {
            inOnAssetLoaded(asset);
        });
        
        Engine::GetThreadPool().QueueTask([asset, inPath, inOnAssetLoaded, this]()
        {
            ZoneScoped;
            Coroutine<void, void, false> loadCoroutine = asset->Load(inPath);
            loadCoroutine.GetOnCoroutineComplete().Bind([this, inPath, asset]()
            {
                std::scoped_lock<std::recursive_mutex> lock(myMutex);
                check(asset);
                
                AssetContainer<AssetType>* container = GetContainerForAssetType<AssetType>();
                asset->myIsValid = true;
                container->myPendingOnAssetLoaded[inPath].Invoke();
                container->myPendingOnAssetLoaded.erase(inPath);
            });
            loadCoroutine.Resume();
        });
    }

    template<typename AssetType>
    AssetType* GetAssetSynchronous(const std::filesystem::path& inPath)
    {
        std::scoped_lock<std::recursive_mutex> lock(myMutex);
        AssetContainer<AssetType>* container = GetContainerForAssetType<AssetType>();
        
        if (container->myAssets.contains(inPath))
        {
            AssetType* asset = static_cast<AssetType*>(container->myAssets.at(inPath));
            return asset;
        }

        AssetType* asset = new AssetType();
        asset->myPath = inPath;
        Coroutine<void, void, false> loadCoroutine = asset->Load(inPath);
        // TODO: Is this even guaranteed to work correctly? if we return an awaitable it will be considered ready here even though it isnt.
        loadCoroutine.Resume();
        asset->myIsValid = true;
        check(asset);
        container->myAssets.insert({inPath, asset});
        return asset;
    }

    template<typename AssetType>
    AssetContainer<AssetType>* GetContainerForAssetType()
    {
        std::scoped_lock<std::recursive_mutex> lock(myMutex);
        for(IAssetContainer* container : myContainers)
        {
            if(AssetContainer<AssetType>* castedContainer = dynamic_cast<AssetContainer<AssetType>*>(container))
                return castedContainer;
        }

        myContainers.Add(new AssetContainer<AssetType>());
        return static_cast<AssetContainer<AssetType>*>(myContainers.Last());
    }
    
private:
    inline static std::unordered_map<std::string, std::filesystem::path> myFilenameToPathLUT{};

    std::recursive_mutex myMutex{};
    List<IAssetContainer*> myContainers{};
};
