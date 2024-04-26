#pragma once

#include "FuncCtor.hpp"

template<typename>
class MemberFuncCtor;

template<typename Class, typename ReturnType, typename... ArgTypes>
class MemberFuncCtor<ReturnType(Class::*)(ArgTypes...)> final : public FuncCtor<ReturnType(ArgTypes...)>
{
private:
    using MemberInstance = Class*;
    using MemberFunction = ReturnType(Class::*)(ArgTypes...);

public:
    explicit MemberFuncCtor(const MemberFunction inFunction, const MemberInstance inInstance)
        : myInstance(inInstance), myFunction(inFunction)
    { }

    ~MemberFuncCtor() = default;

    bool operator==(const FuncCtor<ReturnType(ArgTypes...)>& inOther) override
    {
        const MemberFuncCtor* member = dynamic_cast<const MemberFuncCtor*>(&inOther);

        if(!member)
            return false;

        return member->myInstance == myInstance && member->myFunction == myFunction;
    }

    ReturnType operator()(ArgTypes&&... inArgs) override
    {
        check(myInstance && "Unbound delegate cannot be invoked.");
        check(myFunction && "Unbound delegate cannot be invoked.");

        return (myInstance->*myFunction)(std::forward<ArgTypes>(inArgs)...);
    }

private:
    const MemberInstance myInstance;
    const MemberFunction myFunction;
};