#include "EnginePch.h"
#include "AssetSerializer.h"

#include "Engine/Engine.h"
#include "Engine/AssetRegistry/Asset2.h"
#include "Engine/AssetRegistry/AssetRegistry2.h"
#include "Engine/Serialization/BinarySerializer.h"

bool AssetSerializer::SerializesType(const Type* inType) const
{
    const List<TypeTemplateArgument>& templateArguments = inType->GetTemplateArguments();
    
    if (templateArguments.IsEmpty())
        return false;
    
    return inType->GetName().contains("std::shared_ptr") && templateArguments.First().myType->DerivesFrom<Asset2>();
}

void AssetSerializer::Serialize(void* inInstance, const Type* inType, BinarySerializer* inSerializer)
{
    const List<TypeTemplateArgument>& templateArguments = inType->GetTemplateArguments();
    
    SharedPtr<Asset2>* asset = static_cast<SharedPtr<Asset2>*>(inInstance);
    
    const Type* assetType = templateArguments.First().myType;
    
    std::string sourcePath = asset->get()->GetSourcePath().string();
    inSerializer->SerializeString(sourcePath);
    
    if (inSerializer->IsReading())
    {
        *asset = Engine::GetEngineSystem<AssetRegistry2>().GetAsset(sourcePath,assetType);
    }
}