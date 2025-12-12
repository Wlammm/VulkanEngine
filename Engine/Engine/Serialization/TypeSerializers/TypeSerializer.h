#pragma once
#include "Engine/Reflection/ReflectionSystem.h"

class BinarySerializer;
class Type;


/*
 * NOTES:
 * Be very careful when writing your own type serializer as you will manually need to handle versioning of the data it writes.
 *
 * The case of when a type serializer is removed the property will be skipped but handled. If you change data you need to either support
 * backwards compatibility yourself or remove everything saved with it.
 */

class TypeSerializer
{
public:
    virtual ~TypeSerializer() = default;
    virtual void Serialize(void* inInstance, const Type* inType, BinarySerializer* inSerializer) = 0;
    
    
    virtual bool SerializesType(const Type* inType) const = 0;
    
    static void RegisterSerializers()
    {
        const Type* typeSerializerClass = ReflectionSystem::GetType<TypeSerializer>();
        for (const Type* entry : typeSerializerClass->GetDerivedTypes())
        {
            mySerializers.Add(entry->CreateInstance<TypeSerializer>());
        }
    }
    
    static TypeSerializer* GetSerializer(const Type* inType)
    {
        for (TypeSerializer* serializer : mySerializers)
        {
            if (serializer->SerializesType(inType))
                return serializer;
        }
        return nullptr;
    }

private:
    
    
    inline static List<TypeSerializer*> mySerializers;
};
