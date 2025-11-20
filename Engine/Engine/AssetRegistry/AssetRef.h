#pragma once

template<typename AssetType>
class AssetHolder
{
public:
    ~AssetHolder()
    {
        myAsset.Unload();
    }
    
private:
    AssetType myAsset;
};

template<typename AssetType>
using AssetRef = std::shared_ptr<AssetHolder<AssetType>>;