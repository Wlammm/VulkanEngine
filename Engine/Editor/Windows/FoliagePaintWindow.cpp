#include "EditorPch.h"
#include "FoliagePaintWindow.h"

#include "Engine/Engine.h"
#include "Engine/Rendering/FoliageSystem.h"

FoliagePaintWindow::FoliagePaintWindow()
    : EditorWindow("Foliage Paint", true)
{
}

void FoliagePaintWindow::Tick()
{
    FoliageSystem& foliage = Engine::GetEngineSystem<FoliageSystem>();

    const int typeCount = foliage.GetTypeCount();
    if (typeCount == 0)
    {
        ImGui::TextDisabled("No foliage types registered.");
        return;
    }

    if (myActiveType >= typeCount)
        myActiveType = 0;

    ImGui::Checkbox("Enable painting", &myPaintEnabled);
    ImGui::TextDisabled("LMB paints, hold Shift to erase. RMB still pans the camera.");
    ImGui::Separator();

    if (ImGui::BeginCombo("Foliage type", foliage.GetTypeName(myActiveType).c_str()))
    {
        for (int i = 0; i < typeCount; ++i)
        {
            const bool selected = (i == myActiveType);
            if (ImGui::Selectable(foliage.GetTypeName(i).c_str(), selected))
                myActiveType = i;
            if (selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    ImGui::SliderFloat("Brush radius", &myBrushRadius, 50.0f, 2500.0f, "%.0f");
    ImGui::SliderFloat("Strength", &myStrength, 0.01f, 1.0f);
    ImGui::Checkbox("Erase", &myErase);

    ImGui::Separator();
    ImGui::Text("Instances: %u / %u", foliage.GetNumInstances(), foliage.GetCapacity());

    if (ImGui::Button("Regenerate"))
        foliage.RegenerateInstances();

    ImGui::SameLine();
    if (ImGui::Button("Fill active type"))
    {
        foliage.SetTypeDensity(myActiveType, 1.0f);
        foliage.RegenerateInstances();
    }

    ImGui::SameLine();
    if (ImGui::Button("Clear active type"))
    {
        foliage.SetTypeDensity(myActiveType, 0.0f);
        foliage.RegenerateInstances();
    }

    ImGui::Separator();
    ImGui::TextDisabled("Scalability");
    bool scalabilityChanged = false;
    scalabilityChanged |= ImGui::SliderFloat("Max distance scale", &myMaxDistanceScale, 0.1f, 3.0f);
    scalabilityChanged |= ImGui::SliderFloat("Density scale", &myDensityScale, 0.0f, 1.0f);
    if (scalabilityChanged)
        foliage.SetScalability(myMaxDistanceScale, myDensityScale);
}
