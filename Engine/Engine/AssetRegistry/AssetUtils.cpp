#include "EnginePch.h"
#include "AssetUtils.h"

void AssetUtils::ScanAssetDirectory()
{
    for(const std::filesystem::path& path : std::filesystem::recursive_directory_iterator("./"))
    {
        if(!path.has_extension())
            continue;

        check(!myFilenameToPathLUT.contains(path.filename().string()));
        myFilenameToPathLUT.insert({path.filename().string(), path});
    }

    LOG("AssetRegistry - Scanned %i files", myFilenameToPathLUT.size());
}

SourcePath AssetUtils::GetSourcePathFromAssetName(const std::string& inAssetName)
{
    if(inAssetName == "")
        return "";

    if(!myFilenameToPathLUT.contains(inAssetName))
        return "";
    
    return myFilenameToPathLUT.at(inAssetName);    
}
