#pragma once
#include "EditorWindow.h"

class HierarchyWindow : public EditorWindow
{
public:
    HierarchyWindow();

    void Tick() override;

    void TickInput() override;

private:
    
};
