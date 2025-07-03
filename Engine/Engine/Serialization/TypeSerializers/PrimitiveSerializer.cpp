#include "EnginePch.h"
#include "PrimitiveSerializer.h"

#include "Engine/Serialization/BinaryReader.h"
#include "Engine/Serialization/BinaryWriter.h"

bool PrimitiveSerializer::SerializesType(const Class* inClass) const
{
    ReflectionSystem& reflectionSystem = Engine::GetReflectionSystem();

    if (reflectionSystem.GetClass<int>() == inClass)
        return true;

    if (reflectionSystem.GetClass<float>() == inClass)
        return true;

    if (reflectionSystem.GetClass<double>() == inClass)
        return true;

    if (reflectionSystem.GetClass<char>() == inClass)
        return true;

    if (reflectionSystem.GetClass<unsigned char>() == inClass)
        return true;

    if (reflectionSystem.GetClass<bool>() == inClass)
        return true;

    if (reflectionSystem.GetClass<short>() == inClass)
        return true;

    if (reflectionSystem.GetClass<unsigned short>() == inClass)
        return true;

    return false;
}

void PrimitiveSerializer::Write(void* inInstance, const Class* inClass, BinaryWriter* inWriter)
{
    inWriter->Write(inInstance, inClass->GetSize());
}

void PrimitiveSerializer::Read(void* inInstance, const Class* inClass, BinaryReader* inReader)
{
    inReader->Read(inInstance, inClass->GetSize());
}
