#pragma once
#include "System.h"

class World;

class WorldSystem : public System
{
public:
    WorldSystem(World* inWorld);
    virtual ~WorldSystem() = default;

    virtual void Tick() {};

    World* GetWorld() const;
    
private:
    World* myWorld = nullptr;
};
