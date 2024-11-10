#pragma once

class WorldSystem
{
public:
    virtual ~WorldSystem() = default;

    virtual void Tick() {};
};
