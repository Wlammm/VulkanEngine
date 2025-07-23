#include "EnginePch.h"
#include "MapSerializer.h"

bool MapSerializer::SerializesType(const Type* inType) const
{
    if (inType->GetName().contains("unordered_map"))
    {
        int a= 10;
    }
    return false;
}

void MapSerializer::Serialize(void* inInstance, const Type* inType, BinarySerializer* inSerializer)
{
    
}
