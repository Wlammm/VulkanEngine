#pragma once
#include "EditorWindow.h"

#include <string>

// Editor console: shows the CVarSystem output log and an input line for running commands / cheats.
// Auto-registered with the editor through the reflection system (it derives from EditorWindow).
class ConsoleWindow : public EditorWindow
{
public:
    ConsoleWindow();

    void Tick() override;

private:
    std::string myInputBuffer;
    bool myScrollToBottom = false;
};
