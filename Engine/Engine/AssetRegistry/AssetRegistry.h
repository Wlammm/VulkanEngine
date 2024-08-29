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
    
    template<typename AssetType>
    void GetAssetAsync(const std::filesystem::path& inPath, const Delegate<void(AssetType* inAsset)> inOnAssetLoaded)
    {
        ZoneScoped;
        myMutex.lock();
        if(myLoadedAssets.contains(inPath))
        {
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
