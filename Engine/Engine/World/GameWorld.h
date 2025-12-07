#pragma once
#include "World.h"

class GameWorld : public World, public AutoInit
{
public:
    void Init() override;
    
    void Tick() override;
    
};
