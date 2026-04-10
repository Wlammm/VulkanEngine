#include "EditorPch.h"
#include "HierarchyWindow.h"

#include "Editor.h"
#include "Actors/EditorCameraActor.h"
#include "Editor/EditorSystem/SelectionSystem.h"
#include "Engine/Engine.h"
#include "Engine/ComponentSystem/Actor.h"
#include "Engine/Core/Input.h"
#include "Engine/Reflection/ReflectionSystem.h"
#include "Engine/Utils/ActorUtils.h"
#include "Engine/Utils/String.hpp"
#include "Engine/World/World.h"
#include "Engine/Vulkan/VulkanImGui.h"
#include "World/EditorWorld.h"

HierarchyWindow::HierarchyWindow()
    : EditorWindow("Hierarchy", true)
{
}

void HierarchyWindow::Tick()
{
    ImGui::BeginChild("##HierarchySearchField", ImVec2(0, 20.f * VulkanImGui::GetCurrentDpiScale()), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    
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
        
        if (ImGui::IsWindowHovered() && ImGui::GetIO().MouseClicked[1])
        {
            ImGui::OpenPopup("##HierarchyRightclickPopup");
        }

        const Type* objectType = ReflectionSystem::GetType(actor);

        ImGui::SameLine();
        float rightAlignmentPos = ImGui::GetWindowSize().x - ImGui::CalcTextSize(objectType->GetName().c_str()).x - 25; 
        ImGui::SetCursorPosX(rightAlignmentPos);
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(150, 150, 150, 255));
        ImGui::TextUnformatted(objectType->GetName().c_str());
        ImGui::PopStyleColor();
    }
    
    DrawPopups();
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
    
    
    if (Input::IsKeyPressed(KeyCode::LeftControl) && Input::IsKeyDown(KeyCode::D))
    {
        if (!SelectionSystem::GetSelectedObjects().IsEmpty())
        {
            ActorUtils::DuplicateActor(SelectionSystem::GetSelectedObjects().First());
        }
    }
}

void HierarchyWindow::DrawPopups()
{
    if (ImGui::BeginPopup("##HierarchyRightclickPopup"))
    {
        ON_SCOPE_EXIT([](){ ImGui::EndPopup(); });
        
        if (ImGui::BeginMenu("New"))
        {
            ON_SCOPE_EXIT([](){ ImGui::EndMenu(); });
            
            if (ImGui::Selectable("Empty Actor"))
            {
                SpawnActor(ReflectionSystem::GetType<Actor>());
            }
            
            if (ImGui::BeginMenu("New Actor"))
            {
                ON_SCOPE_EXIT([](){ ImGui::EndMenu(); });

                const Type* baseActorType = ReflectionSystem::GetType<Actor>();
                
                for (const Type* actorType : baseActorType->GetDerivedTypes())
                {
                    if (ImGui::Selectable(actorType->GetName().c_str()))
                    {
                        SpawnActor(actorType);
                    }
                }
            }
        }
    }
}

void HierarchyWindow::SpawnActor(const Type* inType)
{
    Actor* actor = Editor::GetEditorWorld()->SpawnActor(inType, "New " + inType->GetName());
    SelectionSystem::ClearSelection();
    SelectionSystem::SelectObject(actor);

    constexpr float distanceFromCamera = 2'000;
    TransformComponent& editorCameraTransform = Editor::GetEditorWorld()->GetEditorCamera()->GetTransform();
    glm::vec3 newLocation = editorCameraTransform.GetPosition() + editorCameraTransform.GetForward() * distanceFromCamera;
    actor->GetTransform().SetPosition(newLocation);
}
