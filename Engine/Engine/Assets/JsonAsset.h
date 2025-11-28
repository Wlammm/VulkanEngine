#pragma once
#include <nlohmann/json.hpp>

#include "Engine/AssetRegistry/Asset2.h"

class JsonAsset : public Asset2
{
public:
    bool IsCacheValid() const override { return false; };
    
    void LoadPropertiesFromSource() override;

    const nlohmann::json& GetJson() const;
    
private:
    nlohmann::json myJson;
};
