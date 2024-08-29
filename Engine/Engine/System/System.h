#pragma once

class System
{
public:
    System() = default;
    virtual ~System() = default;
    
    virtual void Init() {}
};
