#pragma once
#include "Coroutine.h"

class CoroutineManager
{
public:
    CoroutineManager();
    
    Coroutine<void, void> Load();

    inline static std::thread workerThread;
    inline static std::thread workerThread2;
};

