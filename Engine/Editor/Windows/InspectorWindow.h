#pragma once
#include "EditorWindow.h"

class Component;

class InspectorWindow : public EditorWindow
{
public:
    InspectorWindow();

    void Tick() override;

private:
    void DrawComponentProperties(Component* inComponent);
};
