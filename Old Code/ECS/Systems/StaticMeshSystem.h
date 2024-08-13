#pragma once
#include "ECS/System.h"

class StaticMeshSystem : public System
{
public:
    StaticMeshSystem();

    void Tick() override;
    
};
