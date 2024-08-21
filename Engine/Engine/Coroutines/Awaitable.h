#pragma once
#include <coroutine>

class Awaitable
{
public:
    virtual ~Awaitable() = default;
    virtual void OnAwait(std::coroutine_handle<> inCoroutineHandle) = 0;
    
    constexpr bool await_ready() const noexcept { return false; }
    void await_suspend(std::coroutine_handle<> inCoroutineHandle)
    {
        OnAwait(inCoroutineHandle);
    }
    void await_resume() {}
};
