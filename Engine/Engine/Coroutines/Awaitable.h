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

enum class ThreadType
{
    Main,
    Worker,
    Rendering,
};

namespace Awaitables
{
    class SwitchToThread : public Awaitable
    {
    public:
        SwitchToThread(const ThreadType inThread)
        {
            myThreadType = inThread;
        }
    
        void OnAwait(std::coroutine_handle<> inCoroutineHandle) override;

    private:
        ThreadType myThreadType;
    };

    class WaitForSeconds : public Awaitable
    {
    public:
        WaitForSeconds(const float inSeconds) : mySeconds(inSeconds) {}

        void OnAwait(std::coroutine_handle<> inCoroutineHandle) override;
    
    private:
        float mySeconds;
    };

}