#include "EnginePch.h"
#include "Awaitable.h"

#include "Engine.h"
#include "Core/ThreadPool.h"

void Awaitables::SwitchToThread::OnAwait(std::coroutine_handle<> inCoroutineHandle)
{
    if(myThreadType == ThreadType::Main)
    {
        Engine::TickNextFrame.Bind([inCoroutineHandle](){ inCoroutineHandle.resume();});
    }

    if(myThreadType == ThreadType::Worker)
    {
        Engine::GetThreadPool().QueueTask([inCoroutineHandle]() { inCoroutineHandle.resume(); });
    }
}

void Awaitables::WaitForSeconds::OnAwait(std::coroutine_handle<> inCoroutineHandle)
{
    std::this_thread::sleep_for(std::chrono::duration<float>(mySeconds));
    inCoroutineHandle.resume();
}