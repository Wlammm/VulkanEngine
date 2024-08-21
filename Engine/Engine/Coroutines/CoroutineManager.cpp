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
    std::cout << "Currently on thread: " << std::this_thread::get_id() << "\n";
    std::cout << "Resumed on thread: " << std::this_thread::get_id() << "\n";
    co_return;
}