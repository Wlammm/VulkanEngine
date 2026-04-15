#include "EditorPch.h"
#include "PerformanceEditorWindow.h"

#include "Engine/Debug/PerformanceViewer.h"

PerformanceEditorWindow::PerformanceEditorWindow()
    : EditorWindow("Performance Viewer", true)
{
    
}

void PerformanceEditorWindow::Tick()
{
    PerformanceViewer::RenderImGui();
}