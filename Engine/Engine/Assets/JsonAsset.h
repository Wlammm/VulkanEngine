#pragma once
#include <nlohmann/json.hpp>

#include "Engine/AssetRegistry/Asset.h"

class JsonAsset : public Asset
{
public:
    bool IsCacheValid() const override { return false; };
    
    List<std::string> GetAssetExtensions() const override { return {".json"}; }
    
    void LoadPropertiesFromSource() override;

    const nlohmann::json& GetJson() const;
    
private:
    nlohmann::json myJson;
};
