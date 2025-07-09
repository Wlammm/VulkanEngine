#include "EnginePch.h"
#include "PrimitiveSerializer.h"

#include "Engine/Serialization/BinarySerializer.h"

bool PrimitiveSerializer::SerializesType(const Type* inClass) const
{
    return
        inClass->IsA<int>() ||
        inClass->IsA<unsigned int>() ||
        inClass->IsA<float>() ||
        inClass->IsA<char>() ||
        inClass->IsA<unsigned char>() ||
        inClass->IsA<short>() ||
        inClass->IsA<bool>();
}

void PrimitiveSerializer::Serialize(void* inInstance, const Type* inClass, BinarySerializer* inSerializer)
{
    inSerializer->SerializeBinaryData(inInstance, inClass->GetSize());
}
