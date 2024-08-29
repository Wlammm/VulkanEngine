#pragma once
#include "Coroutines/Coroutine.h"

class AssetRegistry;

class Asset
{
public:
    virtual ~Asset() = default;

    bool IsValid() const;

    virtual Coroutine<void, void, false> Load(const std::filesystem::path inPath) = 0;
    virtual void Unload() = 0;

protected:
    friend class AssetRegistry;
    AssetRegistry* GetAssetRegistry() const;
    
    std::filesystem::path myPath;
    bool myIsValid = false;

private:
    AssetRegistry* myAssetRegistry = nullptr;
};