#pragma once
#include "Engine/Engine.h"
#include "Engine/Reflection/ReflectionSystem.h"

class BinaryReader;
class Class;
class BinaryWriter;

class ITypeSerializer
{
public:
    virtual void Write(void* inInstance, BinaryWriter* inWriter) = 0;
    virtual void Read(void* inInstance, BinaryReader* inReader) = 0;

    const Class* GetClass() const { return myClass; }
    
    static ITypeSerializer* GetSerializer(const Class* inClass)
    {
        if (mySerializers.IsEmpty())
            RegisterSerializers();

        for (ITypeSerializer* serializer : mySerializers)
        {
            if (serializer->GetClass() == inClass)
                return serializer;
        }
        return nullptr;
    }
    
protected:
    const Class* myClass = nullptr;

    
private:
    static void RegisterSerializers()
    {
        for (const Class* entry : Engine::GetReflectionSystem().GetClass<ITypeSerializer>()->GetDerivedClasses())
        {
            mySerializers.Add(entry->CreateInstance<ITypeSerializer>());
        }
    }
    
    inline static List<ITypeSerializer*> mySerializers;
};

template<typename ClassType>
class TypeSerializer : public ITypeSerializer
{
public:
    TypeSerializer()
    {
        myClass = Engine::GetReflectionSystem().GetClass<ClassType>();
    }
};
