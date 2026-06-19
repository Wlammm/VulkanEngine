#pragma once
#include "EditorWindow.h"

/*
 * Settings panel for the foliage brush. The actual painting happens in
 * FoliagePaintTool (an EditorSystem) which reads these settings each frame; this
 * window just exposes them and the scalability sliders.
 */
class FoliagePaintWindow : public EditorWindow
{
public:
    FoliagePaintWindow();

    bool IsPaintEnabled() const { return myPaintEnabled; }
    int GetActiveType() const { return myActiveType; }
    float GetBrushRadius() const { return myBrushRadius; }
    float GetStrength() const { return myStrength; }
    bool IsErase() const { return myErase; }

protected:
    void Tick() override;

private:
    bool myPaintEnabled = false;
    int myActiveType = 0;
    float myBrushRadius = 400.0f;   // world units
    float myStrength = 0.5f;        // 0..1, scaled per-frame while held
    bool myErase = false;

    // Scalability (pushed to the GPU cull pass).
    float myMaxDistanceScale = 1.0f;
    float myDensityScale = 1.0f;
};
