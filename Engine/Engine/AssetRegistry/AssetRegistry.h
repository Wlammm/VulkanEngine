#pragma once
#include "Engine.h"
#include "Core/ThreadPool.h"
#include "Coroutines/Coroutine.h"
#include "Delegates/Delegate.hpp"

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
    
    template<typename AssetType>
    void GetAssetAsync(const std::filesystem::path& inPath, const Delegate<void(AssetType* inAsset)> inOnAssetLoaded)
    {
        ZoneScoped;
        myMutex.lock();
        if(myLoadedAssets.contains(inPath))
        {
            // TODO: This might be an issue if we request more assets in here that might cause a deadlock.
            inOnAssetLoaded(static_cast<AssetType*>(myLoadedAssets[inPath]));
            myMutex.unlock();
            return;
        }

        Engine::GetThreadPool().QueueTask([inPath, inOnAssetLoaded, this]()
        {
            ZoneScoped;
            AssetType* asset = new AssetType();
            asset->myPath = inPath;
            Coroutine<void, void, false> loadCoroutine = asset->Load(inPath);
            loadCoroutine.GetOnCoroutineComplete().Bind([asset, inOnAssetLoaded]()
            {
                asset->myIsValid = true;
                inOnAssetLoaded(asset);
            });
            loadCoroutine.Resume();
            myMutex.lock();
            myLoadedAssets.insert({inPath, asset});
            myMutex.unlock();
        });
        myMutex.unlock();
    }

    template<typename AssetType>
    AssetType* GetAssetSynchronous(const std::filesystem::path& inPath)
    {
        myMutex.lock();
        if(myLoadedAssets.contains(inPath))
        {
            AssetType* asset = static_cast<AssetType*>(myLoadedAssets[inPath]);
            myMutex.unlock();
            return asset;
        }
        
        AssetType* asset = new AssetType();
        asset->myPath = inPath;
        Coroutine<void, void, false> loadCoroutine = asset->Load(inPath);
        loadCoroutine.Resume();
        asset->myIsValid = true;
        myLoadedAssets.insert({inPath, asset});
        myMutex.unlock();
        return asset;
    }

private:
    inline static std::unordered_map<std::string, std::filesystem::path> myFilenameToPathLUT{};
    
    std::mutex myMutex{};
    std::unordered_map<std::filesystem::path, Asset*> myLoadedAssets{};
};
