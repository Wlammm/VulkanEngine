#pragma once
#include <nlohmann/json.hpp>

#include "Engine/AssetRegistry/Asset.h"

class JsonAsset : public Asset
{
public:
    bool IsCacheValid() const override { return false; };
    
    static List<std::string> GetAssetExtensions() { return {".json"}; }
    
    static bool IsExternalAsset() { return true; }
    
    void LoadPropertiesFromSource() override;

    const nlohmann::json& GetJson() const;
    
private:
    nlohmann::json myJson;
};
