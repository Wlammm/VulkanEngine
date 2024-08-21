#pragma once
#include "Coroutine.h"
#include "Awaitable.h"

class WaitForSeconds : public Awaitable
{
public:
    WaitForSeconds(const float inSeconds) : mySeconds(inSeconds) {}

    void OnAwait(std::coroutine_handle<> inCoroutineHandle) override
    {
        std::this_thread::sleep_for(std::chrono::duration<float>(mySeconds));
    }
    
private:
    float mySeconds;
};

class CoroutineManager
{
public:
    CoroutineManager();

    Coroutine<void, void> Load();

    inline static std::thread workerThread;
    inline static std::thread workerThread2;
};

enum class ThreadType
{
    Main,
    Worker,
    Rendering,
};