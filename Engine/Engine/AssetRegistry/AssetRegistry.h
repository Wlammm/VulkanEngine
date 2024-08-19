#pragma once
#include "Engine.h"
#include "Core/ThreadPool.h"
#include "Delegates/Delegate.hpp"

class Asset;
class AssetRegistry
{
public:
    AssetRegistry();
    ~AssetRegistry();
    
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
            asset->Load(inPath);
            asset->myIsValid = true;
            myMutex.lock();
            myLoadedAssets.insert({inPath, asset});
            myMutex.unlock();
            inOnAssetLoaded(asset);
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
        asset->Load(inPath);
        asset->myIsValid = true;
        myLoadedAssets.insert({inPath, asset});
        myMutex.unlock();
        return asset;
    }

private:
    std::mutex myMutex{};
    std::unordered_map<std::filesystem::path, Asset*> myLoadedAssets{};
};
