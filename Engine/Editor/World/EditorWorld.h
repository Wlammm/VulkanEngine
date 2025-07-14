#pragma once
#include "Engine/World/World.h"

class EditorWorld : public World
{
public:
    void Init() override;
    
    void Tick() override;

    bool myShouldTickComponents = false;
};
