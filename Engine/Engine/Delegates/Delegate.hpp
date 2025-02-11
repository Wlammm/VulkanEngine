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
#define SOURCE_LOCATION_INPUT_ARG char c = 'c'
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
#if DEBUG
            mySourceLocation = inOther.mySourceLocation;
#endif
        }
        else
        {
            myFuncCtor.reset();
#if DEBUG
            mySourceLocation = {};
#endif
        }
    }

    // Move constructor
    Delegate(Delegate&& other) noexcept : myFuncCtor(std::move(other.myFuncCtor))
    {
        other.myFuncCtor.reset();
#if DEBUG
        mySourceLocation = std::move(other.mySourceLocation);
#endif
    }

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
#if DEBUG
                mySourceLocation = inOther.mySourceLocation;
#endif
            }
            else
            {
                myFuncCtor.reset();
#if DEBUG
                mySourceLocation = {};
#endif
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
            inOther.myFuncCtor.reset();
#if DEBUG
            mySourceLocation = inOther.mySourceLocation;
#endif
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

#if DEBUG
    std::source_location GetSourceLocation() const
    {
        return mySourceLocation;
    }
#endif
private:
    std::unique_ptr<FuncCtor> myFuncCtor;
#if DEBUG
    std::source_location mySourceLocation;
#endif
};