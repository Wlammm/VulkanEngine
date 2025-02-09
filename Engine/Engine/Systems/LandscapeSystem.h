#pragma once
#include "System/WorldSystem.h"

class LandscapeSystem final : public WorldSystem
{
public:
    explicit LandscapeSystem(World* inWorld)
        : WorldSystem(inWorld)
    { }

    ~LandscapeSystem() override;

private:
    
};
