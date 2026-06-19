#pragma once
#include "EditorSystem.h"

/*
 * Drives foliage painting from viewport input. Reads brush settings from
 * FoliagePaintWindow, raycasts the heightfield under the cursor, paints the
 * active type's density map, and regenerates the instance pool (throttled).
 */
class FoliagePaintTool : public EditorSystem
{
public:
    void Tick() override;

private:
    bool myDirty = false;
    int myLastRegenFrame = 0;
};
