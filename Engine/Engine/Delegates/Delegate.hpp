#pragma once

#include "Internal/FuncCtor.hpp"
#include "Internal/FreeFuncCtor.hpp"
#include "Internal/MemberFuncCtor.hpp"
#include "Internal/ConstMemberFuncCtor.hpp"
#include "Internal/LambdaFuncCtor.hpp"

template <typename>
class Delegate;

template<typename ReturnType, typename... ArgTypes>
class Delegate<ReturnType(ArgTypes...)> final
{
private:
    using FuncCtor = FuncCtor<ReturnType(ArgTypes...)>;
    
public:
    Delegate()
    {
        myFuncCtor.reset();
    }

    // Move constructor
    Delegate(Delegate&& other) noexcept : myFuncCtor(std::move(other.myFuncCtor)) {}

    template<typename Callable>
    Delegate(Callable&& func)
    {
        // Create a LambdaFuncCtor and store it
        myFuncCtor = std::make_unique<LambdaFuncCtor<ReturnType(ArgTypes...)>>(std::forward<Callable>(func));
    }
    
    Delegate(const Delegate& inOther)
    {
        if(inOther.myFuncCtor)
        {
            myFuncCtor = inOther.myFuncCtor->Clone();
        }
    }

    // Move assignment operator
    Delegate& operator=(Delegate&& inOther) noexcept
    {
        if (this != &inOther)
        {
            myFuncCtor = std::move(inOther.myFuncCtor);
        }
        return *this;
    }

    Delegate(ReturnType(*freeFunction)(ArgTypes...))
    {
        myFuncCtor.reset(new FreeFuncCtor<ReturnType(ArgTypes...)>(freeFunction));
    }

    template<class Class>
    Delegate(ReturnType(Class::*memberFunction)(ArgTypes...), Class* inInstance)
    {
        myFuncCtor.reset(new MemberFuncCtor<ReturnType(Class::*)(ArgTypes...)>(memberFunction, inInstance));
    }

    template<class Class>
    Delegate(ReturnType(Class::*memberFunction)(ArgTypes...) const, Class* inInstance)
    {
        myFuncCtor.reset(new ConstMemberFuncCtor<ReturnType(Class::*)(ArgTypes...) const>(memberFunction, inInstance));
    }

    template<class Class>
    Delegate(ReturnType(Class::*memberFunction)(ArgTypes...), Class& inInstance)
    {
        myFuncCtor.reset(new MemberFuncCtor<ReturnType(Class::*)(ArgTypes...)>(memberFunction, &inInstance));
    }

    template<class Class>
    Delegate(ReturnType(Class::*memberFunction)(ArgTypes...) const, Class& inInstance)
    {
        myFuncCtor.reset(new ConstMemberFuncCtor<ReturnType(Class::*)(ArgTypes...) const>(memberFunction, &inInstance));
    }

    bool operator==(const Delegate& inOther) const
    {
        return *myFuncCtor == *inOther.myFuncCtor;
    }

    ~Delegate() = default;

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
};