#pragma once

#include "FuncCtor.hpp"
#include "Engine/Core/CheckDefine.hpp"
#include <functional>
#include <utility>

template<typename>
class LambdaFuncCtor;

template<typename ReturnType, typename... ArgTypes>
class LambdaFuncCtor<ReturnType(ArgTypes...)> final : public FuncCtor<ReturnType(ArgTypes...)>
{
private:
    using LambdaType = std::function<ReturnType(ArgTypes...)>;

public:
    explicit LambdaFuncCtor(LambdaType inLambda)
        : myLambda(std::move(inLambda)) { }

    LambdaFuncCtor(const LambdaFuncCtor& other)
        : myLambda(other.myLambda) { }

    LambdaFuncCtor(LambdaFuncCtor&& other) noexcept
        : myLambda(std::move(other.myLambda))
    { }

    LambdaFuncCtor& operator=(const LambdaFuncCtor& other)
    {
        if (this != &other)
        {
            myLambda = other.myLambda;
        }
        return *this;
    }

    LambdaFuncCtor& operator=(LambdaFuncCtor&& other) noexcept
    {
        if (this != &other)
        {
            myLambda = std::move(other.myLambda);
        }
        return *this;
    }
    
    ~LambdaFuncCtor() = default;

    bool operator==(const FuncCtor<ReturnType(ArgTypes...)>& inOther) const override
    {
        return false; // Lambda comparison isn't feasible in general.
    }

    ReturnType operator()(ArgTypes&&... inArgs) override
    {
        check(myLambda && "Unbound delegate cannot be invoked.");

        return myLambda(std::forward<ArgTypes>(inArgs)...);
    }

    std::unique_ptr<FuncCtor<ReturnType(ArgTypes...)>> Clone() const override
    {
        auto result = std::make_unique<LambdaFuncCtor>(*this);
        return result;
    }

private:
    LambdaType myLambda;
};