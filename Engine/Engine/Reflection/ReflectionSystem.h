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

private:
    friend class GeneratedReflectionData;
    void AddClass(const std::string& inClassName, const std::string& inFullName);

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
