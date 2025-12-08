#include "EditorPch.h"
#include "PreviousWorldsSystem.h"

#include "Engine/Engine.h"
#include "Engine/AssetRegistry/AssetRegistry.h"
#include "Engine/Serialization/BinarySerializer.h"
#include "Engine/World/World.h"
#include "World/EditorWorld.h"

PreviousWorldsSystem::PreviousWorldsSystem()
{
    if (std::filesystem::exists(mySavePath))
    {
        BinarySerializer serializer(mySavePath, BinarySerializer::Mode::Read);
        serializer.SerializeType(myPreviousWorlds);
        serializer.Close();
    }
    
    Engine::OnWorldChangedDelegate.Bind(&PreviousWorldsSystem::OnWorldChanged, this);
}

PreviousWorldsSystem::~PreviousWorldsSystem()
{
    Engine::OnWorldChangedDelegate.UnBind(&PreviousWorldsSystem::OnWorldChanged, this);
    
    BinarySerializer serializer(mySavePath, BinarySerializer::Mode::Write);
    serializer.SerializeType(myPreviousWorlds);
    serializer.Close();
}

void PreviousWorldsSystem::Tick()
{
    ImGui::SetNextWindowSize(ImVec2(500, 200));
    const glm::vec2 windowPos = Engine::GetRenderResolution() * 0.5 - glm::vec2(250, 100);
    
    ImGui::SetNextWindowPos(ImVec2(windowPos.x, windowPos.y));
    std::string popupName = ImGui::GetCenteredWindowName("Open Previous World");
    
    if (ImGui::BeginPopupModal(popupName.c_str(), nullptr, ImGuiWindowFlags_NoResize))
    {
        ON_SCOPE_EXIT([](){ImGui::EndPopup();});
        
        if (!ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow) &&
        ImGui::IsMouseClicked(ImGuiMouseButton_Left))
        {
            ImGui::CloseCurrentPopup();
        }
        
        for (int i = myPreviousWorlds.size() - 1; i >= 0; --i)
        {
            if (ImGui::CenteredSelectable(myPreviousWorlds[i].stem().string().c_str(), false, 0, ImVec2(500, 40)))
            {        
                Engine::SetWorld(AssetRegistry::Get()->GetAsset<EditorWorld>(myPreviousWorlds[i]));
                ImGui::CloseCurrentPopup();
                return;
            }
            ImGui::Separator();
        }
    }
    
    DO_ONCE(
        if (myPreviousWorlds.size() > 0)
            ImGui::OpenPopup(popupName.c_str());
    );
}

void PreviousWorldsSystem::OnWorldChanged(SharedPtr<World> inPreviousWorld)
{
    SharedPtr<World> currentWorld = Engine::GetWorld();
    
    if (!currentWorld)
        return;
    
    if (currentWorld->GetSourcePath() == "" || currentWorld->GetSourcePath() == World::EmptyWorldPath)
        return;
    
    if (int index = myPreviousWorlds.FindIndex(currentWorld->GetSourcePath().string()); index != -1)
    {
        myPreviousWorlds.RemoveIndex(index);
    }
    else if (myPreviousWorlds.size() == myMaxWorldsSaved)
    {
        myPreviousWorlds.RemoveIndex(0);
    }
    
    myPreviousWorlds.Emplace(currentWorld->GetSourcePath());
}
