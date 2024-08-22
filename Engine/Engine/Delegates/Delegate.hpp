#pragma once

#include "Internal/FuncCtor.hpp"
#include "Internal/FreeFuncCtor.hpp"
#include "Internal/MemberFuncCtor.hpp"
#include "Internal/ConstMemberFuncCtor.hpp"
#include "Internal/LambdaFuncCtor.hpp"

#if DEBUG
#include <source_location>
#define SOURCE_LOCATION_INPUT_ARG const std::source_location inSourceLocation = std::source_location::current()
#define SOURCE_LOCATION_ASSIGNMENT mySourceLocation = inSourceLocation
#else
#define SOURCE_LOCATION_INPUT_ARG
#define SOURCE_LOCATION_ASSIGNMENT
#endif

template <typename>
class Delegate;

template<typename ReturnType, typename... ArgTypes>
class Delegate<ReturnType(ArgTypes...)> final
{
private:
    using FuncCtor = FuncCtor<ReturnType(ArgTypes...)>;
    
public:
    explicit Delegate(SOURCE_LOCATION_INPUT_ARG)
    {
        SOURCE_LOCATION_ASSIGNMENT;
        myFuncCtor.reset();
    }

    Delegate(const Delegate& inOther)
    {
        if (inOther.myFuncCtor)
        {
            myFuncCtor = inOther.myFuncCtor->Clone();
            mySourceLocation = inOther.mySourceLocation;
        }
        else
        {
            myFuncCtor.reset();
            mySourceLocation = {};
        }
    }

    // Move constructor
    Delegate(Delegate&& other) noexcept : myFuncCtor(std::move(other.myFuncCtor)) {}

    ~Delegate() = default;
    
    template<typename Callable, typename = std::enable_if_t<!std::is_same_v<std::decay_t<Callable>, Delegate>>>
    Delegate(Callable&& func, SOURCE_LOCATION_INPUT_ARG)
    {
        SOURCE_LOCATION_ASSIGNMENT;
        myFuncCtor = std::make_unique<LambdaFuncCtor<ReturnType(ArgTypes...)>>(std::forward<Callable>(func));
    }
    
    Delegate& operator=(const Delegate& inOther)
    {
        if (this != &inOther)
        {
            if (inOther.myFuncCtor)
            {
                myFuncCtor = inOther.myFuncCtor->Clone();
                mySourceLocation = inOther.mySourceLocation;
            }
            else
            {
                myFuncCtor.reset();
                mySourceLocation = {};
            }
        }
        return *this;
    }

    // Move assignment operator
    Delegate& operator=(Delegate&& inOther) noexcept
    {
        if (this != &inOther)
        {
            myFuncCtor = std::move(inOther.myFuncCtor);
            mySourceLocation = inOther.mySourceLocation;
        }
        return *this;
    }

    explicit Delegate(ReturnType(*freeFunction)(ArgTypes...), SOURCE_LOCATION_INPUT_ARG)
    {
        SOURCE_LOCATION_ASSIGNMENT;
        myFuncCtor.reset(new FreeFuncCtor<ReturnType(ArgTypes...)>(freeFunction));
    }

    template<class Class>
    Delegate(ReturnType(Class::*memberFunction)(ArgTypes...), Class* inInstance, SOURCE_LOCATION_INPUT_ARG)
    {
        SOURCE_LOCATION_ASSIGNMENT;
        myFuncCtor.reset(new MemberFuncCtor<ReturnType(Class::*)(ArgTypes...)>(memberFunction, inInstance));
    }

    template<class Class>
    Delegate(ReturnType(Class::*memberFunction)(ArgTypes...) const, Class* inInstance, SOURCE_LOCATION_INPUT_ARG)
    {
        SOURCE_LOCATION_ASSIGNMENT;
        myFuncCtor.reset(new ConstMemberFuncCtor<ReturnType(Class::*)(ArgTypes...) const>(memberFunction, inInstance));
    }

    template<class Class>
    Delegate(ReturnType(Class::*memberFunction)(ArgTypes...), Class& inInstance, SOURCE_LOCATION_INPUT_ARG)
    {
        SOURCE_LOCATION_ASSIGNMENT;
        myFuncCtor.reset(new MemberFuncCtor<ReturnType(Class::*)(ArgTypes...)>(memberFunction, &inInstance));
    }

    template<class Class>
    Delegate(ReturnType(Class::*memberFunction)(ArgTypes...) const, Class& inInstance, SOURCE_LOCATION_INPUT_ARG)
    {
        SOURCE_LOCATION_ASSIGNMENT;
        myFuncCtor.reset(new ConstMemberFuncCtor<ReturnType(Class::*)(ArgTypes...) const>(memberFunction, &inInstance));
    }

    bool operator==(const Delegate& inOther) const
    {
        if (!myFuncCtor || !inOther.myFuncCtor)
            return false;

        return *myFuncCtor == *inOther.myFuncCtor;
    }

    ReturnType Invoke(ArgTypes... inArgs) const
    {
        check(myFuncCtor && "Cannot invoke unbound delegate.");

        return myFuncCtor->operator()(std::forward<ArgTypes>(inArgs)...);
    }

    ReturnType operator()(ArgTypes... inArgs) const
    {
        check(myFuncCtor && "Cannot invoke unbound delegate.");

        return Invoke(std::forward<ArgTypes>(inArgs)...);
    }

private:
    std::unique_ptr<FuncCtor> myFuncCtor;
#if DEBUG
    std::source_location mySourceLocation;
#endif
};