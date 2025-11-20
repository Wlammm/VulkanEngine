#include "EnginePch.h"
#include "AssetSerializer.h"

#include "Engine/AssetRegistry/Asset.h"
#include "Engine/Serialization/BinarySerializer.h"

bool AssetSerializer::SerializesType(const Type* inType) const
{
    return inType->IsA<Asset>();
}

void AssetSerializer::Serialize(void* inInstance, const Type* inType, BinarySerializer* inSerializer)
{
    std::string fullTypeName = inType->GetFullName();
    inSerializer->SerializeString(fullTypeName);

    
}
