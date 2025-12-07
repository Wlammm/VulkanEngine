#pragma once
#include "EditorSystem.h"
#include "Engine/AssetRegistry/AssetDefines.h"

class World;

class PreviousWorldsSystem : public EditorSystem
{
public:
    PreviousWorldsSystem();
    ~PreviousWorldsSystem() override;
    
    void Tick() override;
    
private:
    void OnWorldChanged(SharedPtr<World> inPreviousWorld);
    
    const std::string mySavePath ="Editor/PreviousWorldsSystem.info";
    const int myMaxWorldsSaved = 5;

    // Last index is most previously used.
    List<SourcePath> myPreviousWorlds;
};
