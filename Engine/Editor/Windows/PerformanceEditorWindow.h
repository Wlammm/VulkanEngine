#pragma once
#include "EditorWindow.h"

class PerformanceEditorWindow : public EditorWindow
{
public:
    PerformanceEditorWindow();
    
    void Tick() override;
};
