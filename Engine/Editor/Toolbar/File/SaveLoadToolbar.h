#pragma once
#include "Editor/EditorSystem/SelectionSystem.h"
#include "Editor/World/EditorWorld.h"
#include "Engine/Engine.h"
#include "Engine/World/World.h"

class SaveLoadToolbar
{
public:
    META(EditorMenuItem("File/Save"))
    static void Save()
    {
        Engine::GetWorld()->SaveToFile("Worlds/WorldSave.world");
    }

    META(EditorMenuItem("File/Load"))    
    static void Load()
    {
        SelectionSystem::ClearSelection();
        
        Engine::SetWorld(MakeShared<EditorWorld>());

        Engine::GetWorld()->LoadFromFile("Worlds/WorldSave.world");
    }
};
