#pragma once
#include <nlohmann/json.hpp>

#include "Engine/AssetRegistry/AssetRegistry2.h"

class JsonAsset : public Asset2
{
public:
    static constexpr bool CanAssetBeCached() { return false; };
    
    void LoadPropertiesFromSource() override;

    const nlohmann::json& GetJson() const;
    
private:
    nlohmann::json myJson;
};
