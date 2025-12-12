#pragma once
#include "ImGui_Wrapper.hpp"
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
            if (world->GetSourcePath() == World::EmptyWorldPath)
            {
                SourcePath path = FileDialog::SaveFileDialog("World (.world)\0*.world;");
                
                if (!path.has_extension())
                    path += ".world";
                
                world->SetAssetPath(path);
            }
            
            AssetRegistry::Get()->SaveAsset(world);
        }
    }

    META(EditorMenuItem("File/Load World"))    
    static void LoadWorld()
    {
        SelectionSystem::ClearSelection();

        SourcePath path = FileDialog::OpenFileDialog("World (.world)\0*.world");
        
        if (!std::filesystem::exists(path))
            return;
        
        AssetRegistry::Get()->GetAssetAsync<EditorWorld>(path, [](SharedPtr<EditorWorld> inWorld)
        {
            Engine::SetWorld(inWorld);
        });
        
        ImGui::NotifyInfo("Async operation", "Loading world");
    }
};
