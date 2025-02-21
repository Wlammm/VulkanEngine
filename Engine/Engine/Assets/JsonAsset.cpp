#include "EnginePch.h"
#include "JsonAsset.h"
#include <nlohmann/json.hpp>

Coroutine<void, void, false> JsonAsset::Load(const std::filesystem::path inPath)
{
    myJson = nlohmann::json::parse(inPath.string());
    co_return;
}

void JsonAsset::Unload()
{
    
}

const nlohmann::json& JsonAsset::GetJson() const
{
    return myJson;
}
