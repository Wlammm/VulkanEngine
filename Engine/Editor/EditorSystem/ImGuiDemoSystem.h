#pragma once
#include "EditorSystem.h"

class ImGuiDemoSystem : public EditorSystem
{
public:
    ImGuiDemoSystem();
    
    META(EditorMenuItem("Open ImGui Demo Window"))
    static void OpenImGuiDemoWindow();
    
    void Tick() override;

private:
    inline static bool myIsWindowOpen = false;
};
