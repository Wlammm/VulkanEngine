#pragma once
#include <any>

#include "Engine/Delegates/Delegate.hpp"

class Class;

class MethodArgument
{
    friend class ReflectionSystem;
    friend class GeneratedReflectionData;
public:
    const std::string& GetArgumentName() const;
    const Class* GetArgumentType() const;

private:
    MethodArgument(const std::string& inArgumentName, const Class* inArgumentType);

private:
    std::string myArgumentName;
    const Class* myArgumentType = nullptr;
};

class Method
{
public:
    using InvokerType = Delegate<void*(void*, const List<void*>&)>;
    
    const std::string& GetMethodName() const;

    void* Invoke(void* inInstance, const List<void*>& inArguments) const;

    template<typename ReturnType, typename... Args>
    ReturnType Invoke(void* inInstance, Args&&... inArgs) const
    {
        List<void*> arguments = { (void*)std::addressof(inArgs)... };
        void* result = Invoke(inInstance,  arguments);

        if constexpr (std::is_void_v<ReturnType>)
        {
            return;
        }
        else if constexpr (std::is_reference_v<ReturnType>)
        {
            using RawType = std::remove_reference_t<ReturnType>;
            return *reinterpret_cast<RawType*>(result);
        }
        else
        {
            return *reinterpret_cast<ReturnType*>(result);
        }
    }
    
private:
    Method(const std::string& inMethodName, const Class* inReturnType, const InvokerType& inInvoker, const List<MethodArgument>& inArguments);
    
private:
    friend class ReflectionSystem;
    friend class GeneratedReflectionData;
    
    std::string myMethodName;
    const Class* myReturnType = nullptr;
    List<MethodArgument> myArguments{};

    InvokerType myInvoker;
};
