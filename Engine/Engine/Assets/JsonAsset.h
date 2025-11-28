#pragma once
#include <nlohmann/json.hpp>

#include "Engine/AssetRegistry/Asset2.h"

class JsonAsset : public Asset2
{
public:
    virtual constexpr bool CanAssetBeCached() override { return false; };
    
    void LoadPropertiesFromSource() override;

    const nlohmann::json& GetJson() const;
    
private:
    nlohmann::json myJson;
};
