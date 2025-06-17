#include "EditorPch.h"
#include "ImGuiDemoSystem.h"

#include "Editor/Editor.h"

ImGuiDemoSystem::ImGuiDemoSystem()
{
}

void ImGuiDemoSystem::OpenImGuiDemoWindow()
{
    myIsWindowOpen = true;
}

void ImGuiDemoSystem::Tick()
{
    if (myIsWindowOpen)
        ImGui::ShowDemoWindow(&myIsWindowOpen);
}
