#include "EnginePch.h"
#include "CoroutineManager.h"

#include "Awaitable.h"

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
    std::cout << "Started on thread: " << std::this_thread::get_id() << std::endl;
    co_await Awaitables::SwitchToThread(ThreadType::Worker);
    std::cout << "Resumed on thread: " << std::this_thread::get_id() << std::endl;
    co_await Awaitables::SwitchToThread(ThreadType::Main);
    std::cout << "Ended on thread: " << std::this_thread::get_id() << std::endl;
    co_return;
}