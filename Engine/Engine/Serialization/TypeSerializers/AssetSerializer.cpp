#include "EnginePch.h"
#include "AssetSerializer.h"

#include "Engine/Engine.h"
#include "Engine/AssetRegistry/Asset.h"
#include "Engine/AssetRegistry/AssetRegistry.h"
#include "Engine/Serialization/BinarySerializer.h"

bool AssetSerializer::SerializesType(const Type* inType) const
{
    const List<TypeTemplateArgument>& templateArguments = inType->GetTemplateArguments();
    
    if (templateArguments.IsEmpty())
        return false;
    
    return inType->GetName().contains("std::shared_ptr") && templateArguments.First().myType->DerivesFrom<Asset>();
}

void AssetSerializer::Serialize(void* inInstance, const Type* inType, BinarySerializer* inSerializer)
{
    const List<TypeTemplateArgument>& templateArguments = inType->GetTemplateArguments();
    
    SharedPtr<Asset>* asset = static_cast<SharedPtr<Asset>*>(inInstance);
    
    const Type* assetType = templateArguments.First().myType;
    
    std::string sourcePath;
    if (inSerializer->IsWriting())
    {
        if (asset->get())
            sourcePath = asset->get()->GetSourcePath().string();
    }
    inSerializer->SerializeString(sourcePath);
    
    if (inSerializer->IsReading())
    {
        if (sourcePath == "")
            return;
        
        *asset = AssetRegistry::Get()->GetAsset(sourcePath, assetType);
        
        // TODO: Make this async working. We'd need to be able to subscribe to events when an asset has finished loading..
        
        // Engine::GetEngineSystem<AssetRegistry>().GetAssetAsync(sourcePath, assetType,  [asset](SharedPtr<Asset> inAsset)
        // {
            // *asset = inAsset;
        // });
    }
}