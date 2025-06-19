#pragma once
#include "EditorSystem.h"

class ImGuiDemoSystem : public EditorSystem
{
public:
    ImGuiDemoSystem();
    
    META(EditorMenuItem("ImGui Demo"))
    static void OpenImGuiDemoWindow();
    
    void Tick() override;

private:
    inline static bool myIsWindowOpen = false;
};
