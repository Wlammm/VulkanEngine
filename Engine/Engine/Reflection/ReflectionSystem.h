#pragma once
#include "Class.h"
#include "Engine/System/System.h"

class ReflectionSystem : public System
{
public:
    template<typename ClassType>
    const Class* GetClass() const
    {
        const std::string typeName = typeid(ClassType).name();

        for (const Class& entry : myClasses)
        {
            if (entry.GetFullName() == typeName)
                return &entry;
        }
        
        return nullptr;
    }

    template<typename BaseType>
    const Class* GetClass(const BaseType* inInstance) const
    {
        const std::string typeName = typeid(*inInstance).name();
        for (const Class& entry : myClasses)
        {
            if (entry.GetFullName() == typeName)
                return &entry;
        }
        return nullptr;
    }

private:
    friend class GeneratedReflectionData;

    template<typename ClassType>
    void AddClass(const std::string& inClassName, const std::string& inFullName)
    {
        if constexpr (std::is_default_constructible_v<ClassType> && !std::is_abstract_v<ClassType>)
            myClasses.Add(Class(inClassName, inFullName, []() -> void* { return new ClassType(); }));
        else
            myClasses.Add(Class(inClassName, inFullName, []() -> void* { check(false && "Class is not default constructible or abstract"); return nullptr; }));
    }

    template<typename ClassType>
    Class* GetMutableClass() const
    {
        const std::string typeName = typeid(ClassType).name();
        for (Class& entry : myClasses)
        {
            if (entry.GetFullName() == typeName)
                return &entry;
        }

        return nullptr;
    }

    List<Class> myClasses{};
};
