#pragma once

#include "FuncCtor.hpp"

template<typename>
class ConstMemberFuncCtor;

template <class Class, typename ReturnType, typename... ArgTypes>
class ConstMemberFuncCtor<ReturnType(Class::*)(ArgTypes...) const> final : public FuncCtor<ReturnType(ArgTypes...)>
{
private:
    using ConstMemberInstance = const Class*;
    using ConstMemberFunction = ReturnType(Class::*)(ArgTypes...) const;

public:
    explicit ConstMemberFuncCtor(const ConstMemberFunction inFunction, const ConstMemberInstance inInstance):
        myInstance(inInstance),
        myFunction(inFunction)
    { }

    ~ConstMemberFuncCtor() = default;

    bool operator==(const FuncCtor<ReturnType(ArgTypes...)>& inOther) const override
    {
        // Cant dynamic cast to pointer here so need to do this for const type :/
        try
        {
            const ConstMemberFuncCtor& memberConst = dynamic_cast<const ConstMemberFuncCtor&>(inOther);

            return myFunction == memberConst.myFunction && myInstance == memberConst.myInstance;
        }
        catch (const std::bad_cast&)
        {
            return false;
        }
    }
    
    ReturnType operator()(ArgTypes&&... inArgs) override
    {
        check(myInstance && "Unbound delegate cannot be invoked.");
        check(myFunction && "Unbound delegate cannot be invoked.");

        return (myInstance->*myFunction)(std::forward<ArgTypes>(inArgs)...);
    }

private:
    const ConstMemberInstance myInstance;
    const ConstMemberFunction myFunction;
};