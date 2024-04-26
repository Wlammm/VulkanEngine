#pragma once

#include "FuncCtor.hpp"
#include "FreeFuncCtor.hpp"
#include "MemberFuncCtor.hpp"
#include "ConstMemberFuncCtor.hpp"

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

    Delegate(const Delegate& inOther)
    {
        myFuncCtor = inOther.myFuncCtor;
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
        return myFuncCtor == inOther.myFuncCtor;
    }

    ~Delegate() = default;

    ReturnType Invoke(ArgTypes... inArgs)
    {
        check(myFuncCtor && "Cannot invoke unbound delegate.");

        return myFuncCtor->operator()(std::forward<ArgTypes>(inArgs)...);
    }

    ReturnType operator()(ArgTypes... inArgs)
    {
        check(myFuncCtor && "Cannot invoke unbound delegate.");

        return Invoke(std::forward<ArgTypes>(inArgs)...);
    }

private:
    std::shared_ptr<FuncCtor> myFuncCtor;
};