#include "EditorPch.h"
#include "HierarchyWindow.h"

#include "Editor/EditorSystem/SelectionSystem.h"
#include "Engine/Engine.h"
#include "Engine/ComponentSystem/Actor.h"
#include "Engine/Core/Input.h"
#include "Engine/Reflection/ReflectionSystem.h"
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

    for (int objectIndex = 0; objectIndex < Engine::GetWorld()->GetAllActors().size(); ++objectIndex)
    {
        Actor* actor = Engine::GetWorld()->GetAllActors()[objectIndex].Get();

        const std::string lowerCaseActorName = String::ToLowerCopy(actor->GetName());
        if (lowerCaseSearch != "" && lowerCaseActorName.find(lowerCaseSearch) == std::string::npos)
            continue;
        
        std::string nameID = actor->GetName() + "##" + std::to_string(objectIndex);
        if (ImGui::Selectable(nameID.c_str(), SelectionSystem::IsObjectSelected(actor), ImGuiSelectableFlags_AllowDoubleClick))
        {
            if (!Input::IsKeyPressed(KeyCode::LeftControl))
                SelectionSystem::ClearSelection();

            if (SelectionSystem::IsObjectSelected(actor))
                SelectionSystem::DeselectObject(actor);
            else
                SelectionSystem::SelectObject(actor);
        }

        const Type* objectType = ReflectionSystem::GetType(actor);

        ImGui::SameLine();
        float rightAlignmentPos = ImGui::GetWindowSize().x - ImGui::CalcTextSize(objectType->GetName().c_str()).x - 25; 
        ImGui::SetCursorPosX(rightAlignmentPos);
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(150, 150, 150, 255));
        ImGui::TextUnformatted(objectType->GetName().c_str());
        ImGui::PopStyleColor();
    }
}

void HierarchyWindow::TickInput()
{
    if (Input::IsKeyDown(KeyCode::Delete))
    {
        for (Actor* object : SelectionSystem::GetSelectedObjects())
        {
            object->Destroy();
        }
        SelectionSystem::ClearSelection();
    }
    
}
