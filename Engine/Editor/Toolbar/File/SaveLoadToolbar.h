#pragma once
#include "Editor/EditorSystem/SelectionSystem.h"
#include "Editor/World/EditorWorld.h"
#include "Engine/Engine.h"
#include "Engine/AssetRegistry/AssetRegistry.h"
#include "Engine/World/World.h"
#include "Editor/Utils/FileDialog.h"

class SaveLoadToolbar
{
public:
    META(EditorMenuItem("File/Save World"))
    static void SaveWorld()
    {
        if (SharedPtr<World> world = Engine::GetWorld())
        {
            if (world->GetSourcePath() == "NewWorld.world")
            {
                SourcePath path = FileDialog::SaveFileDialog("Scene (.scene)\0*.scene;");
                world->SetAssetPath(path);
            }
            
            Engine::GetWorld()->ResaveAsset();
        }
    }

    META(EditorMenuItem("File/Load World"))    
    static void LoadWorld()
    {
        SelectionSystem::ClearSelection();

        SourcePath path = FileDialog::OpenFileDialog("Scene (.scene)\0*.scene");
        
        if (!std::filesystem::exists(path))
            return;
        
        Engine::SetWorld(AssetRegistry::Get()->GetAsset<EditorWorld>(path));
    }
};
