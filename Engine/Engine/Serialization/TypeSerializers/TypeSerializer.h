#pragma once
#include "Engine/Reflection/ReflectionSystem.h"

class BinarySerializer;
class Class;

class TypeSerializer
{
public:
    virtual ~TypeSerializer() = default;
    virtual void Serialize(void* inInstance, const Class* inClass, BinarySerializer* inSerializer) = 0;
    
    virtual bool SerializesType(const Class* inClass) const = 0;
    
    static TypeSerializer* GetSerializer(const Class* inClass)
    {
        if (mySerializers.IsEmpty())
            RegisterSerializers();

        for (TypeSerializer* serializer : mySerializers)
        {
            if (serializer->SerializesType(inClass))
                return serializer;
        }
        return nullptr;
    }

private:
    static void RegisterSerializers()
    {
        const Class* typeSerializerClass = ReflectionSystem::GetClass<TypeSerializer>();
        for (const Class* entry : typeSerializerClass->GetDerivedClasses())
        {
            mySerializers.Add(entry->CreateInstance<TypeSerializer>());
        }
    }
    
    inline static List<TypeSerializer*> mySerializers;
};
