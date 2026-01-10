#include "EnginePch.h"
#include "ReflectionSystem.h"

#include "Engine/Serialization/BinarySerializer.h"

const List<Type*>& ReflectionSystem::GetAllTypes()
{
    return myTypes;
}

bool ReflectionSystem::CopyProperties(void* inSource, void* inTarget, const Type* inType)
{
    std::ostringstream stream;
    {
        BinarySerializer serializer(stream, BinarySerializer::Mode::Write);
        serializer.SerializeType(inSource, inType, false);
    }
    {
        BinarySerializer serializer(stream, BinarySerializer::Mode::Read);
        serializer.SerializeType(inTarget, inType, false);
        
        return serializer.WasLastTypeSerializationFullyComplete();
    }
}
