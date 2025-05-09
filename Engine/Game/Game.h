#pragma once

class Game
{
public:
    Game();
    ~Game();
    
    static void StaticTick();
    
    void Tick();
    
private:
    inline static Game* myInstance = nullptr;
};
