#pragma once

template<typename>
class FuncCtor;

template<typename ReturnType, typename... ArgTypes>
class FuncCtor<ReturnType(ArgTypes...)>
{
public:
    virtual ~FuncCtor() = default;

    virtual bool operator==(const FuncCtor<ReturnType(ArgTypes...)>& inOther) = 0;

    virtual ReturnType operator()(ArgTypes&&... inArgs) = 0;
};