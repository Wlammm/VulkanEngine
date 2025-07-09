#pragma once
#include "Engine/Reflection/ReflectionSystem.h"

class BinarySerializer;
class Type;

class TypeSerializer
{
public:
    virtual ~TypeSerializer() = default;
    virtual void Serialize(void* inInstance, const Type* inType, BinarySerializer* inSerializer) = 0;
    
    virtual bool SerializesType(const Type* inType) const = 0;
    
    static TypeSerializer* GetSerializer(const Type* inType)
    {
        if (mySerializers.IsEmpty())
            RegisterSerializers();

        for (TypeSerializer* serializer : mySerializers)
        {
            if (serializer->SerializesType(inType))
                return serializer;
        }
        return nullptr;
    }

private:
    static void RegisterSerializers()
    {
        const Type* typeSerializerClass = ReflectionSystem::GetClass<TypeSerializer>();
        for (const Type* entry : typeSerializerClass->GetDerivedTypes())
        {
            mySerializers.Add(entry->CreateInstance<TypeSerializer>());
        }
    }
    
    inline static List<TypeSerializer*> mySerializers;
};
