#pragma once
#include "World.h"

class GameWorld : public World
{
public:
    void Init() override;
    
    void Tick() override;
};
