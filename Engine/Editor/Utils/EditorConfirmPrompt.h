#pragma once
#include "Engine/Delegates/Delegate.hpp"

class EditorConfirmPrompt
{
public:
    EditorConfirmPrompt() = default;
    EditorConfirmPrompt(const std::string& inTitle, const std::string& inBody, const Delegate<void()>& inCallback);

    void Show() const;
    void Update();

private:
    std::string myTitle{};
    std::string myBody{};
    Delegate<void()> myCallback{};
};
