#include "EnginePch.h"
#include "AssetRegistry.h"

#include "Asset.h"
#include "Engine/Engine.h"
#include "Engine/Core/ThreadPool.h"

AssetRegistry::AssetRegistry()
{
    check(!mySingleton);
    mySingleton = this;
}

AssetRegistry::~AssetRegistry()
{
    mySingleton = nullptr;
}

void AssetRegistry::GetAssetAsync(const SourcePath& inSourcePath, const Type* inType,
                                  const Delegate<void(SharedPtr<Asset> inAsset)>& inOnCompleteDelegate)
{
    if (SharedPtr<Asset> asset = TryGetLoadedAsset(inSourcePath))
    {
        inOnCompleteDelegate.Invoke(asset);
        return;
    }

    Engine::GetThreadPool().QueueTask([inSourcePath, inType, this, inOnCompleteDelegate]()
    {
        SharedPtr<Asset> asset = GetAssetSynchronous(inSourcePath, inType);

        Engine::TickNextFrame.Bind([asset, inOnCompleteDelegate]()
        {
            inOnCompleteDelegate.Invoke(asset);
        });
    });
}

SharedPtr<Asset> AssetRegistry::GetAssetSynchronous(const SourcePath& inSourcePath, const Type* inType)
{
    ZoneScoped;
    ZoneText(inSourcePath.filename().string().c_str(), inSourcePath.filename().string().size());
    
    const SourcePath path = TryRedirectPath(inSourcePath);
    
    SharedPtr<Asset> asset = TryGetLoadedAsset(path);
    if (asset != nullptr)
        return asset;

    // TODO: We want some guarantee that loaded assets stay valid throughout the entire frame. Currently we have none. So assets can get unloaded while we're trying to load them in-between our checks..
    
    myPendingAssets.Lock();
    if (myPendingAssets.Contains(path))
    {
        myPendingAssets.Unlock();
        // It is not guaranteed the asset is still in here but that is fine. In that case we know its already been loaded.
        return WaitForPendingAsset(path);
    }
    else
    {
        myPendingAssets.Add(path);
        myPendingAssets.Unlock();
    }

    asset = inType->CreateSharedPtr<Asset>();

    if (inType->CallStaticMethodRecursive<bool>("IsExternalAsset"))
    {
        asset = LoadExternalAsset(path, inType);
    }
    else
    {
        asset = LoadInternalAsset(path, inType);
    }
    
    myPendingAssets.Remove(path);

    return asset;
}

SourcePath AssetRegistry::TryRedirectPath(const SourcePath& inSourcePath) const
{
    if (inSourcePath.string().starts_with("Shaders/"))
        return String::Replace(inSourcePath.string(), "Shaders/", "../Engine/Engine/Shaders/");
        
    return inSourcePath;
}

CachePath AssetRegistry::SourceToCachePath(const SourcePath& inSourcePath) const
{
    return "AssetCache/" + inSourcePath.string() + ".cache";
}

void AssetRegistry::OnAssetRemoved(Asset* inAsset)
{
    std::scoped_lock lock(myMutex);
    myLoadedAssets.erase(inAsset->GetSourcePath());
}

void AssetRegistry::AddLoadedAsset(SharedPtr<Asset> inAsset, const Type* inType)
{
    inAsset->SetIsValid(true);
    inAsset->SetAssetRegistry(this);
    inAsset->SetType(inType);

    std::scoped_lock lock(myMutex);

    myLoadedAssets.insert({inAsset->GetSourcePath(), inAsset});
}

SharedPtr<Asset> AssetRegistry::WaitForPendingAsset(const SourcePath& inSourcePath)
{
    // TODO: With the current setup we might have multiple worker threads just spinlocking waiting for one thread to finish loading an asset instead of doing other productive work. Fix that somehow.
    ZoneScoped;
    while (myPendingAssets.Contains(inSourcePath))
    {
        Sleep(1);
    }
    
    std::scoped_lock lock(myMutex);

    // TODO: Can we assure the asset hasn't been removed here already if someone immediately throws away the only reference to the asset?
    return myLoadedAssets.at(inSourcePath).lock();
}

