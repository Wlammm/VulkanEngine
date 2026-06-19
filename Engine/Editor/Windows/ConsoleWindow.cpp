#include "EditorPch.h"
#include "ConsoleWindow.h"

#include "Engine/Engine.h"
#include "Engine/Console/CVarSystem.h"

#include <imgui_stdlib.h>

ConsoleWindow::ConsoleWindow()
    : EditorWindow("Console", true)
{
}

void ConsoleWindow::Tick()
{
    CVarSystem& cvarSystem = Engine::GetEngineSystem<CVarSystem>();

    // Scrollable output log, leaving room for the input line at the bottom.
    const float inputHeight = ImGui::GetFrameHeightWithSpacing();
    ImGui::BeginChild("ConsoleOutput", ImVec2(0, -inputHeight), false, ImGuiWindowFlags_HorizontalScrollbar);
    for (const std::string& line : cvarSystem.GetOutputLog())
        ImGui::TextUnformatted(line.c_str());

    if (myScrollToBottom)
    {
        ImGui::SetScrollHereY(1.0f);
        myScrollToBottom = false;
    }
    ImGui::EndChild();

    ImGui::Separator();

    ImGui::SetNextItemWidth(-1.0f);
    const ImGuiInputTextFlags flags = ImGuiInputTextFlags_EnterReturnsTrue;
    if (ImGui::InputText("##ConsoleInput", &myInputBuffer, flags))
    {
        if (!myInputBuffer.empty())
        {
            cvarSystem.Execute(myInputBuffer);
            myInputBuffer.clear();
            myScrollToBottom = true;
        }

        // Keep focus on the input box so commands can be entered back-to-back.
        ImGui::SetKeyboardFocusHere(-1);
    }
}
