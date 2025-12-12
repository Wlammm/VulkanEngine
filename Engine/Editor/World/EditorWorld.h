#pragma once
#include "Engine/World/World.h"

class EditorCameraActor;

class EditorWorld : public World
{
public:
    void Init() override;
    
    void Tick() override;

    bool myShouldTickComponents = false;

    EditorCameraActor* GetEditorCamera() const;
    
private:
    EditorCameraActor* myEditorCamera = nullptr;
};
