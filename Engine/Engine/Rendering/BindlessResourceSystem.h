#pragma once
#include "ECS/System.h"

class BindlessResourceSystem : public System
{
public:
    BindlessResourceSystem();

    void Tick() override final;

private:
    
};
