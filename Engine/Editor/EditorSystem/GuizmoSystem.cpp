#include "EditorPch.h"
#include "GuizmoSystem.h"

#include "Editor.h"
#include "SelectionSystem.h"
#include "Actors/EditorCameraActor.h"
#include "Engine/Engine.h"
#include "Engine/Core/Input.h"
#include "Windows/Viewport.h"
#include "World/EditorWorld.h"

void GuizmoSystem::Tick()
{
    // TODO: Find a better system for handling when we should and shouldnt take input in the editor.
    if (Input::IsKeyPressed(MouseButton::Right))
        return;
    
    if (Input::IsKeyDown(KeyCode::Q))
    {
        myOperation = (ImGuizmo::OPERATION)0;
    }
    
    if (Input::IsKeyDown(KeyCode::W))
    {
        myOperation = ImGuizmo::OPERATION::TRANSLATE;
    }

    if (Input::IsKeyDown(KeyCode::E))
    {
        myOperation = ImGuizmo::OPERATION::ROTATE;
    }

    if (Input::IsKeyDown(KeyCode::R))
    {
        myOperation = ImGuizmo::OPERATION::SCALE;
    }
}

void GuizmoSystem::DrawGuizmo()
{
    List<Actor*> selectedActors = SelectionSystem::GetSelectedObjects();
    if (selectedActors.IsEmpty())
        return;

    Actor* selectedActor = selectedActors[0];
    
    EditorCameraActor* camera = Editor::GetEditorWorld()->GetEditorCamera();
    if (!camera)
        return;
    
    Viewport* viewport = Editor::GetWindow<Viewport>();
    const ImVec2& pos = viewport->GetViewportPos();
    const ImVec2& size = viewport->GetViewportSize();
    
    glm::mat4 view = glm::inverse(camera->GetTransform().GetMatrix());
    glm::mat4 proj = camera->GetCameraComponent().GetProjection();

    // Vulkan to OpenGL fix (imguizmo requirement)
    proj[1][1] *= -1.0f;

    glm::mat4 objectMatrix = selectedActor->GetTransform().GetMatrix();

    ImGuizmo::SetDrawlist(ImGui::GetWindowDrawList());
    ImGuizmo::SetRect(pos.x, pos.y, size.x, size.y);

    if (ImGuizmo::Manipulate(
            glm::value_ptr(view),
            glm::value_ptr(proj),
            myOperation,
            (myOperation == ImGuizmo::SCALE ? ImGuizmo::LOCAL : mySpace),
            glm::value_ptr(objectMatrix)))
    {
        glm::vec3 modPos, modRot, modScale;
        ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(objectMatrix), &modPos.x, &modRot.x, &modScale.x);
        
        selectedActor->GetTransform().SetPosition(modPos);
        selectedActor->GetTransform().SetRotationDeg(modRot);
        selectedActor->GetTransform().SetScale(modScale);
    }
}
