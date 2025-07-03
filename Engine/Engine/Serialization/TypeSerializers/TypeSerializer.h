#pragma once
#include "Engine/Engine.h"
#include "Engine/Reflection/ReflectionSystem.h"

class BinaryReader;
class Class;
class BinaryWriter;

class TypeSerializer
{
public:
    virtual ~TypeSerializer() = default;
    virtual void Write(void* inInstance, const Class* inClass, BinaryWriter* inWriter) = 0;
    virtual void Read(void* inInstance, const Class* inClass, BinaryReader* inReader) = 0;
    
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
        const Class* typeSerializerClass = Engine::GetReflectionSystem().GetClass<TypeSerializer>();
        for (const Class* entry : typeSerializerClass->GetDerivedClasses())
        {
            mySerializers.Add(entry->CreateInstance<TypeSerializer>());
        }
    }
    
    inline static List<TypeSerializer*> mySerializers;
};
