#include "EnginePch.h"
#include "AssetRegistry.h"

#include "Asset.h"
#include "Engine.h"
#include "Core/ThreadPool.h"

AssetRegistry::AssetRegistry()
{
    
}

AssetRegistry::~AssetRegistry()
{
    myMutex.lock();

    for(auto [path, asset] : myLoadedAssets)
    {
        asset->Unload();
    }
    myLoadedAssets.clear();
    
    myMutex.unlock();
}