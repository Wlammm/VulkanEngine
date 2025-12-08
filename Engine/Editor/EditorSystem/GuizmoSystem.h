#pragma once
#include "EditorSystem.h"
#include "ImGuizmo.h"

class GuizmoSystem : public EditorSystem
{
public:
    void Tick() override;

    void DrawGuizmo();
    
private:
    ImGuizmo::OPERATION myOperation = (ImGuizmo::OPERATION::TRANSLATE);
    ImGuizmo::MODE mySpace = ImGuizmo::MODE::WORLD;
};
