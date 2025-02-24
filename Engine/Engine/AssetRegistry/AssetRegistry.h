#pragma once
#include "Asset.h"
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
    void RegisterTemporaryAsset(const std::filesystem::path& inAssetPath, Asset* inAsset);

    bool HasAsset(const std::filesystem::path& inAssetPath) const;

    template <typename AssetType>
    void GetAssetAsync(const std::filesystem::path& inPath, const Delegate<void(AssetType* inAsset)> inOnAssetLoaded)
    {
        ZoneScoped;
        std::scoped_lock lock(myMutex);
        
        if (myPendingOnAssetLoaded.contains(inPath))
        {
            AssetType* asset = static_cast<AssetType*>(myAssets.at(inPath));
            myPendingOnAssetLoaded[inPath].Bind([inOnAssetLoaded, asset]()
            {
                inOnAssetLoaded(asset);
            });
            return;
        }

        if(myAssets.contains(inPath))
        {
            inOnAssetLoaded(static_cast<AssetType*>(myAssets.at(inPath)));
            return;
        }

        AssetType* asset = new AssetType();
        asset->myPath = inPath;
        asset->myAssetRegistry = this;
        myAssets.insert({inPath, asset});
        
        myPendingOnAssetLoaded.insert({inPath, {}});
        myPendingOnAssetLoaded[inPath].Bind([inOnAssetLoaded, asset]()
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
                
                asset->myIsValid = true;
                myPendingOnAssetLoaded[inPath].Invoke();
                myPendingOnAssetLoaded.erase(inPath);
            });
            loadCoroutine.Resume();
        });
    }

    template<typename AssetType>
    AssetType* GetAssetSynchronous(const std::filesystem::path& inPath)
    {
        std::scoped_lock<std::recursive_mutex> lock(myMutex);
        if (myAssets.contains(inPath))
        {
            AssetType* asset = static_cast<AssetType*>(myAssets.at(inPath));
            return asset;
        }

        AssetType* asset = new AssetType();
        asset->myPath = inPath;
        Coroutine<void, void, false> loadCoroutine = asset->Load(inPath);
        // TODO: Is this even guaranteed to work correctly? if we return an awaitable it will be considered ready here even though it isnt.
        loadCoroutine.Resume();
        asset->myIsValid = true;
        check(asset);
        myAssets.insert({inPath, asset});
        return asset;
    }

private:
    inline static std::unordered_map<std::string, std::filesystem::path> myFilenameToPathLUT{};

    std::recursive_mutex myMutex{};
    std::unordered_map<std::filesystem::path, Asset*> myAssets{};
    std::unordered_map<std::filesystem::path, MulticastDelegate<void()>> myPendingOnAssetLoaded{};
};
