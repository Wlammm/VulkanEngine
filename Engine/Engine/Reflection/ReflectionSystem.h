#pragma once
#include "Type.h"
#include "Engine/Core/UniquePtr.h"

template<typename T>
concept SizedType = requires { sizeof(T); };

class ReflectionSystem 
{
public:
    static const List<Type*>& GetAllTypes();

    static const Type* GetTypeByName(const std::string& inName)
    {
        for (const Type* entry : myTypes)
        {
            if (entry->GetName() == inName)
                return entry;
        }
        return nullptr;
    }

    static const Type* GetTypeByFullName(const std::string& inFullName)
    {
        for (const Type* entry : myTypes)
        {
            if (entry->GetFullName() == inFullName)
                return entry;
        }
        return nullptr;
    }

    template <typename ClassType>
    static const Type* GetType()
    {
        const std::string typeName = typeid(ClassType).name();

        // TODO: Find a faster way than string comparisons.
        for (const Type* entry : myTypes)
        {
            if (entry->GetFullName() == typeName)
                return entry;
        }

        return nullptr;
    }

    template <typename ClassType>
    static const Type* GetType(const ClassType* inInstance)
    {
        const std::string typeName = typeid(*inInstance).name();
        for (const Type* entry : myTypes)
        {
            if (entry->GetFullName() == typeName)
                return entry;
        }
        return nullptr;
    }

private:
    friend class GeneratedReflectionData;

    template <typename ClassType>//
    static void AddType(const std::string& inTypeName, const std::string& inFullName)
    {
        constexpr size_t classSize = []() constexpr
        {
            if constexpr (std::is_void_v<ClassType> || std::is_function_v<ClassType> || !SizedType<ClassType>)
                return size_t(0);
            else
                return sizeof(std::remove_reference_t<ClassType>);
        }();

        // Make sure we dont have duplicate classes.
        for (const Type* entry : myTypes)
        {
            check(entry->myFullName != inFullName);
        }

        Delegate<void(IUniquePtr*)> uniquePtrFactory = nullptr;
        if constexpr (!std::is_abstract_v<ClassType> && std::is_default_constructible_v<ClassType> && !std::is_const_v<ClassType> && !std::is_array_v<ClassType>)
        {
            uniquePtrFactory = [](IUniquePtr* inPtr)
            {
                UniquePtr<ClassType>* castedPtr = static_cast<UniquePtr<ClassType>*>(inPtr);
                *castedPtr = MakeUnique<ClassType>();
            };
        }
        
        Delegate<SharedPtr<void>()> sharedPtrFactory = nullptr;
        if constexpr (!std::is_abstract_v<ClassType> && std::is_default_constructible_v<ClassType>)
        {
            if constexpr (std::is_const_v<ClassType>)
            {
                sharedPtrFactory = []() -> SharedPtr<void>
                {
                    return std::static_pointer_cast<void>(MakeShared<std::remove_const_t<ClassType>>());
                };
            }
            else
            {
                sharedPtrFactory = []() -> SharedPtr<void>
                {
                    return std::static_pointer_cast<void>(MakeShared<ClassType>());
                };
            }
            
        }

        if constexpr (std::is_default_constructible_v<ClassType> && !std::is_abstract_v<ClassType>)
        {
            myTypes.Add(new Type(
                inTypeName, 
                inFullName, 
                classSize, 
                std::is_trivially_copyable_v<ClassType>,
                []() -> void*
                {
                    return new typename std::remove_const<ClassType>::type();
                },
                [](void* destination)
                {
                    new (destination) typename std::remove_const<ClassType>::type();
                }, 
                uniquePtrFactory, 
                sharedPtrFactory));
        }
        else
        {
            myTypes.Add(new Type(
                inTypeName, 
                inFullName, 
                classSize, 
                std::is_trivially_copyable_v<ClassType>,
                []() -> void*
                {
                    check(false && "Class is not default constructible or abstract. Make sure you have implemented an empty constructor and all pure virtual methods!");
                    return nullptr;
                },
            [](void*)
                {
                    check(false && "PlacementNew not supported for this type!");
                }, 
                uniquePtrFactory, 
                sharedPtrFactory));
        }
    }

    template <typename ClassType>
    static const Type* GetOrCreateType(const std::string& inTypeName)
    {
        const Type* retrievedClass = GetType<ClassType>();
        if (retrievedClass)
            return retrievedClass;

        AddType<ClassType>(inTypeName, typeid(ClassType).name());
        std::cout << "Creating class " << inTypeName << std::endl;
        return myTypes.Last();
    }

    template <typename ClassType>
    static Type* GetMutableType()
    {
        const std::string typeName = typeid(ClassType).name();
        for (Type* entry : myTypes)
        {
            if (entry->GetFullName() == typeName)
                return entry;
        }

        return nullptr;
    }

    inline static List<Type*> myTypes{};
};