#pragma once
#include "Class.h"

class ReflectionSystem 
{
public:
    static const List<Class*>& GetAllClasses();

    static const Class* GetClassByName(const std::string& inName)
    {
        for (const Class* entry : myClasses)
        {
            if (entry->GetName() == inName)
                return entry;
        }
        return nullptr;
    }

    static const Class* GetClassByFullName(const std::string& inFullName)
    {
        for (const Class* entry : myClasses)
        {
            if (entry->GetFullName() == inFullName)
                return entry;
        }
        return nullptr;
    }

    template <typename ClassType>
    static const Class* GetClass()
    {
        const std::string typeName = typeid(ClassType).name();

        for (const Class* entry : myClasses)
        {
            if (entry->GetFullName() == typeName)
                return entry;
        }

        return nullptr;
    }

    template <typename ClassType>
    static const Class* GetClass(const ClassType* inInstance)
    {
        const std::string typeName = typeid(*inInstance).name();
        for (const Class* entry : myClasses)
        {
            if (entry->GetFullName() == typeName)
                return entry;
        }
        return nullptr;
    }

private:
    friend class GeneratedReflectionData;

    template <typename ClassType>
    static void AddClass(const std::string& inClassName, const std::string& inFullName)
    {
        constexpr size_t classSize = []() constexpr
        {
            if constexpr (std::is_void_v<ClassType> || std::is_function_v<ClassType>)
                return size_t(0);
            else
                return sizeof(std::remove_reference_t<ClassType>);
        }();

        // Make sure we dont have duplicate classes.
        for (const Class* entry : myClasses)
        {
            check(entry->myFullName != inFullName);
        }

        if constexpr (std::is_default_constructible_v<ClassType> && !std::is_abstract_v<ClassType>)
            myClasses.Add(new Class(inClassName, inFullName, classSize, std::is_trivially_copyable_v<ClassType>,
                []() -> void* { return new typename std::remove_const<ClassType>::type(); }));
        else
            myClasses.Add(new Class(inClassName, inFullName, classSize, std::is_trivially_copyable_v<ClassType>,
                []() -> void*
                {
                    check(false && "Class is not default constructible or abstract. Make sure you have implemented an empty constructor and all pure virtual methods!");
                    return nullptr;
                }));
    }

    template <typename ClassType>
    static const Class* GetOrCreateClass(const std::string& inClassName)
    {
        const Class* retrievedClass = GetClass<ClassType>();
        if (retrievedClass)
            return retrievedClass;

        AddClass<ClassType>(inClassName, typeid(ClassType).name());
        std::cout << "Creating class " << inClassName << std::endl;
        return myClasses.Last();
    }

    template <typename ClassType>
    static Class* GetMutableClass()
    {
        const std::string typeName = typeid(ClassType).name();
        for (Class* entry : myClasses)
        {
            if (entry->GetFullName() == typeName)
                return entry;
        }

        return nullptr;
    }

    inline static List<Class*> myClasses{};
};
