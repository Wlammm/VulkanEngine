#pragma once
#include "ReflectedClass.h"
#include "Engine/System/System.h"

class ReflectionSystem : public System
{
public:
    template<typename ClassType>
    const ReflectedClass* GetClass() const
    {
        const std::string typeName = typeid(ClassType).name();

        for (const ReflectedClass& entry : myClasses)
        {
            if (entry.GetFullName() == typeName)
                return &entry;
        }
        
        return nullptr;
    }

private:
    friend class GeneratedReflectionData;
    void AddClass(const std::string& inClassName, const std::string& inFullName);

    template<typename ClassType>
    ReflectedClass* GetMutableClass() const
    {
        const std::string typeName = typeid(ClassType).name();
        for (ReflectedClass& entry : myClasses)
        {
            if (entry.GetFullName() == typeName)
                return &entry;
        }

        return nullptr;
    }

    List<ReflectedClass> myClasses{};
};
