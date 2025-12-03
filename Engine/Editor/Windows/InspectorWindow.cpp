#include "EditorPch.h"
#include "InspectorWindow.h"

#include "Editor/EditorSystem/SelectionSystem.h"
#include "Engine/Engine.h"
#include "Engine/Components/TransformComponent.h"
#include "Engine/ComponentSystem/Actor.h"
#include "Engine/ComponentSystem/Component.h"
#include "Engine/Reflection/Type.h"
#include "Engine/Reflection/ReflectionSystem.h"
#include "ImGui/ImGuiPropertyDrawer.h"

InspectorWindow::InspectorWindow()
    : EditorWindow("Inspector", true)
{
}

void InspectorWindow::Tick()
{
    const List<Actor*> selectedActors = SelectionSystem::GetSelectedObjects();

    if (selectedActors.IsEmpty())
    {
        ImGui::Text("Select an object to view details.");
        return;
    }

    if (selectedActors.size() > 1)
    {
        ImGui::Text("Inspector does not support multiple selected objects.");
        return;
    }

    Actor* selectedActor = selectedActors.First();

    ImGui::Text("Name: ");
    ImGui::SameLine();
    std::string objectName = selectedActor->GetName();
    if (ImGui::InputText("##inspectorObjectName", &objectName))
    {
        selectedActor->SetName(objectName);
    }

    ImGui::Separator();

    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    DrawComponentProperties(&selectedActor->GetTransform());

    for (Component* component : selectedActor->GetAllComponents())
    {
        if (component->IsA<TransformComponent>())
            continue;

        DrawComponentProperties(component);
    }
}

void InspectorWindow::DrawComponentProperties(Component* inComponent)
{
    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    const Type* componentClass = ReflectionSystem::GetType(inComponent);
    bool isOpen = ImGui::CollapsingHeader(componentClass->GetName().c_str());

    if (isOpen)
    {
        for (const Field& field : componentClass->GetFieldsWithMetadata("SerializeField"))
        {
            if (ImGuiPropertyDrawer::DrawProperty(field, inComponent))
            {
                if (field.HasMetadata("OnInspectorChangedEvent"))
                {
                    // If you crash here it's because you've forgotten to add a methodName to your OnInspectorChangedEvent or the method is private and missing AllowPrivateAccess tag.
                    const std::string methodName = field.GetMetadataArgs("OnInspectorChangedEvent").First();
                    componentClass->GetMethodRecursive(methodName)->Invoke(inComponent);
                }
            }
        }
    }
}