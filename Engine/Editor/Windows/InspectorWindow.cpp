#include "EditorPch.h"
#include "InspectorWindow.h"

#include "ImGuiPropertyDrawer.h"
#include "SelectionSystem.h"
#include "Engine/Engine.h"
#include "Engine/Components/TransformComponent.h"
#include "Engine/ComponentSystem/Component.h"
#include "Engine/ComponentSystem/GameObject.h"
#include "Engine/Reflection/Class.h"
#include "Engine/Reflection/ReflectionSystem.h"

InspectorWindow::InspectorWindow()
    : EditorWindow("Inspector", true)
{
}

void InspectorWindow::Tick()
{
    const List<GameObject*> selectedObjects = SelectionSystem::GetSelectedObjects();

    if (selectedObjects.IsEmpty())
    {
        ImGui::Text("Select an object to view details.");
        return;
    }

    if (selectedObjects.size() > 1)
    {
        ImGui::Text("Inspector does not support multiple selected objects.");
        return;
    }

    GameObject* selectedObject = selectedObjects.First();

    ImGui::Text("Name: ");
    ImGui::SameLine();
    std::string objectName = selectedObject->GetName();
    if (ImGui::InputText("##inspectorObjectName", &objectName))
    {
        selectedObject->SetName(objectName);
    }

    ImGui::Separator();

    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    DrawComponentProperties(selectedObject->GetComponent<TransformComponent>());

    for (Component* component : selectedObject->GetComponents())
    {
        if (component->IsA<TransformComponent>())
            continue;

        DrawComponentProperties(component);
    }
}

void InspectorWindow::DrawComponentProperties(Component* inComponent)
{
    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    const Class* componentClass = Engine::GetEngineSystem<ReflectionSystem>().GetClass(inComponent);
    bool isOpen = ImGui::CollapsingHeader(componentClass->GetName().c_str());

    if (isOpen)
    {
        for (const Field& field : componentClass->GetFieldsWithMetadata("ExposeToEditor"))
        {
            ImGuiPropertyDrawer::DrawProperty(field, inComponent);
        }
    }
}