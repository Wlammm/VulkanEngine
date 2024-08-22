#pragma once

#include "FuncCtor.hpp"

template<typename>
class FreeFuncCtor;

template<typename ReturnType, typename... ArgTypes>
class FreeFuncCtor<ReturnType(ArgTypes...)> final : public FuncCtor<ReturnType(ArgTypes...)>
{
private:
    using FreeFunction = ReturnType(*)(ArgTypes...);
    
public:
    explicit FreeFuncCtor(const FreeFunction inFunction)
        : myFunction(inFunction)
    { }

    ~FreeFuncCtor() = default;

    bool operator==(const FuncCtor<ReturnType(ArgTypes...)>& inOther) const override
    {
        const FreeFuncCtor* free = dynamic_cast<const FreeFuncCtor*>(&inOther);

        if(!free)
            return false;

        return free->myFunction == myFunction;
    }

    ReturnType operator()(ArgTypes&&... inArgs) override
    {
        check(myFunction && "Unbound delegate cannot be invoked.");

        return myFunction(std::forward<ArgTypes>(inArgs)...);
    }

    std::unique_ptr<FuncCtor<ReturnType(ArgTypes...)>> Clone() const override
    {
        return std::make_unique<FreeFuncCtor>(*this);
    }
    
private:
    const FreeFunction myFunction;
};