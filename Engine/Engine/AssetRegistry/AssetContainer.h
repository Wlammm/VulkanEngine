#pragma once

class IAssetContainer
{
public:
    IAssetContainer() = default;
    virtual ~IAssetContainer() = default;
    virtual void UnloadAllAssets() = 0;
};

template<typename AssetType>
class AssetContainer : public IAssetContainer
{
public:
    std::unordered_map<std::filesystem::path, UniquePtr<AssetType>> myAssets{};
    std::unordered_map<std::filesystem::path, MulticastDelegate<void()>> myPendingOnAssetLoaded{};

    void UnloadAllAssets() override
    {
        for(const auto& [path, asset] : myAssets)
        {
            asset->Unload();
        }
        myAssets.clear();
    }
};