#include "EnginePch.h"
#include "JsonAsset.h"
#include <nlohmann/json.hpp>

#include "Engine/AssetRegistry/Asset.h"

void JsonAsset::LoadPropertiesFromSource()
{
    Asset::LoadPropertiesFromSource();
    
    try
    {
        std::ifstream file;
        file.open(GetSourcePath());
        file >> myJson;
        file.close();
    }
    catch(nlohmann::detail::exception exception)
    {
        LOG_ERROR("Failed to parse json: %s", exception.what());
        check(false);
    }
}

const nlohmann::json& JsonAsset::GetJson() const
{
    return myJson;
}
