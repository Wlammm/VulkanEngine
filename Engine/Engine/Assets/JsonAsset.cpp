#include "EnginePch.h"
#include "JsonAsset.h"
#include <nlohmann/json.hpp>

Coroutine<void, void, false> JsonAsset::Load(const std::filesystem::path inPath)
{
    check(std::filesystem::exists(inPath));

    try
    {
        std::ifstream file;
        file.open(inPath);
        file >> myJson;
        file.close();
    }
    catch(nlohmann::detail::exception exception)
    {
        LOG_ERROR("Failed to parse json: %s", exception.what());
        check(false);
    }
    
    co_return;
}

void JsonAsset::Unload()
{
    
}

const nlohmann::json& JsonAsset::GetJson() const
{
    return myJson;
}
