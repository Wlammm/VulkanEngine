#pragma once
#include "Engine/Core/AutoInit.h"

class Game : public AutoInit
{
public:
    Game();
    ~Game() override;

    void Init() override;
    
    static void StaticTick();
    
    void Tick();
    
private:
    inline static Game* myInstance = nullptr;
};
