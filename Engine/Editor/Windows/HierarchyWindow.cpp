#include "EditorPch.h"
#include "HierarchyWindow.h"

#include "Editor/EditorSystem/SelectionSystem.h"
#include "Engine/Engine.h"
#include "Engine/ComponentSystem/GameObject.h"
#include "Engine/Core/Input.h"
#include "Engine/Utils/String.hpp"
#include "Engine/World/World.h"

HierarchyWindow::HierarchyWindow()
    : EditorWindow("Hierarchy", true)
{
}

void HierarchyWindow::Tick()
{
    ImGui::BeginChild("##HierarchySearchField", ImVec2(0, 20), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    
    static std::string searchField = "";
    ImGui::SetNextItemWidth(ImGui::GetWindowWidth() - 0.5f);
    ImGui::InputTextWithHint("##hierarchysearchfield", "Search", &searchField);
    ImGui::EndChild();
    
    const std::string lowerCaseSearch = String::ToLowerCopy(searchField);

    for (int objectIndex = 0; objectIndex < Engine::GetWorld().GetComponentSystem().GetAllGameObjects().size(); ++objectIndex)
    {
        GameObject* gameObject = Engine::GetWorld().GetComponentSystem().GetAllGameObjects()[objectIndex];

        const std::string lowerCaseGameObjectName = String::ToLowerCopy(gameObject->GetName());
        if (lowerCaseSearch != "" && lowerCaseGameObjectName.find(lowerCaseSearch) == std::string::npos)
            continue;
        
        std::string nameID = gameObject->GetName() + "##" + std::to_string(objectIndex);
        if (ImGui::Selectable(nameID.c_str(), SelectionSystem::IsObjectSelected(gameObject), ImGuiSelectableFlags_AllowDoubleClick))
        {
            if (!Input::IsKeyPressed(KeyCode::LeftControl))
                SelectionSystem::ClearSelection();

            if (SelectionSystem::IsObjectSelected(gameObject))
                SelectionSystem::DeselectObject(gameObject);
            else
                SelectionSystem::SelectObject(gameObject);
        }
    }
}
