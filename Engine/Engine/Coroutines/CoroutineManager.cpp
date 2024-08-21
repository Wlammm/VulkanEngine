#include "EnginePch.h"
#include "CoroutineManager.h"

CoroutineManager::CoroutineManager()
{
    auto c = Load();
    while(c.HasYieldValue())
    {
        std::cout << c.ConsumeYieldValue() << std::endl;
        if(!c.IsDone())
            c.Resume();
    }

    if(c.HasReturnValue())
    {
        std::cout << c.ConsumeReturnValue() << std::endl;
    }
    
    int a = 10;
}

Coroutine<void, void> CoroutineManager::Load()
{
    std::cout << "Hello" << "\n";
    co_await WaitForSeconds(10);
    std::cout << "Hiii" << "\n";
    co_return;
}