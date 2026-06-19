#include "EditorPch.h"
#include "FoliagePaintTool.h"

#include "Editor.h"
#include "Windows/FoliagePaintWindow.h"
#include "Windows/Viewport.h"
#include "Engine/Engine.h"
#include "Engine/Components/CameraComponent.h"
#include "Engine/Core/Input.h"
#include "Engine/Rendering/FoliageSystem.h"
#include "Engine/World/World.h"

void FoliagePaintTool::Tick()
{
    FoliagePaintWindow* window = Editor::GetWindow<FoliagePaintWindow>();
    if (!window || !window->IsPaintEnabled())
        return;

    Viewport* viewport = Editor::GetWindow<Viewport>();
    if (!viewport)
        return;

    // The normalized position is clamped to [0,1] (fine for the camera ray helpers),
    // so gate on the real cursor-over-image test instead.
    const glm::vec2 mouse = viewport->GetNormalizedMousePositionInViewport();
    const bool inViewport = viewport->IsMouseOverViewport();

    // Cursor brush indicator (approximate screen radius; the painted result is the truth).
    if (inViewport)
    {
        const ImVec2 mousePos = ImGui::GetMousePos();
        float pixelRadius = window->GetBrushRadius() * 0.08f;
        pixelRadius = pixelRadius < 6.0f ? 6.0f : (pixelRadius > 300.0f ? 300.0f : pixelRadius);
        ImGui::GetForegroundDrawList()->AddCircle(mousePos, pixelRadius, IM_COL32(80, 220, 120, 200), 32, 2.0f);
    }

    // Right mouse pans the camera — don't paint while navigating.
    if (Input::IsKeyPressed(MouseButton::Right) || !inViewport)
        return;

    CameraComponent* camera = Engine::GetWorld()->GetMainCamera();
    if (!camera)
        return;

    FoliageSystem& foliage = Engine::GetEngineSystem<FoliageSystem>();
    const int frame = static_cast<int>(Engine::GetFrameIndex());

    if (Input::IsKeyPressed(MouseButton::Left))
    {
        const glm::vec3 origin = camera->ScreenToNearPlane(mouse);
        const glm::vec3 direction = camera->GetRayDirectionFromScreen(mouse);

        glm::vec3 hit;
        if (foliage.RaycastHeightfield(origin, direction, hit))
        {
            const bool erase = window->IsErase() || Input::IsKeyPressed(KeyCode::LeftShift);
            foliage.PaintDensity(window->GetActiveType(), { hit.x, hit.z }, window->GetBrushRadius(), window->GetStrength() * 0.1f, !erase);
            myDirty = true;
        }
    }

    // Regenerate on release, or periodically while painting so the field updates live.
    const bool released = Input::IsKeyUp(MouseButton::Left);
    if (myDirty && (released || frame - myLastRegenFrame >= 8))
    {
        foliage.RegenerateInstances();
        myLastRegenFrame = frame;
        if (released)
            myDirty = false;
    }
}
