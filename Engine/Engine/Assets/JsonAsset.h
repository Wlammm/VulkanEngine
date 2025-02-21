#pragma once
#include <nlohmann/json.hpp>

#include "AssetRegistry/Asset.h"

class JsonAsset : public Asset
{
public:
    Coroutine<void, void, false> Load(const std::filesystem::path inPath) override;

    void Unload() override;
    
    const nlohmann::json& GetJson() const;
    
private:
    nlohmann::json myJson;
};
