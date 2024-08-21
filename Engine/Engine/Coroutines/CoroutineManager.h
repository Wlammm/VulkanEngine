#pragma once
#include <coroutine>

/*
 *  initial_suspend return type declares if the coroutine should be entered when it is constructed. std::suspend_never will run it immediately while std::suspend_always will yield it immediately.
 *  return_void is called if the return type is void. If there is a value return_value will be called. 
 */

template<typename ReturnType>
class PromiseReturnTypeImplementation
{
public:
    void return_value(ReturnType inValue)
    {
        myReturnValue = inValue;
        myHasReturnValue = true;
    }

    ReturnType myReturnValue;
    bool myHasReturnValue = false;
};

template<>
class PromiseReturnTypeImplementation<void>
{
public:
    void return_void() {}

    // These are only here so we don't get compile errors in the Coroutine class if we use this implementation.
    char myReturnValue;
    bool myHasReturnValue = false;
};


template<typename ReturnType, typename YieldType>
class Coroutine 
{
    // We use this type to make sure we aren't creating member variables of type void. A char will be used instead.
    using InternalReturnType = std::conditional_t<!std::is_void_v<ReturnType>, ReturnType, char>;
    using InternalYieldType = std::conditional_t<!std::is_void_v<YieldType>, YieldType, char>;

public:
    struct promise_type : PromiseReturnTypeImplementation<ReturnType>
    {
        using CoroutineHandle = std::coroutine_handle<promise_type>;
        
        Coroutine get_return_object() { return Coroutine{CoroutineHandle::from_promise(*this)}; }
        std::suspend_never initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void unhandled_exception() {}

        std::suspend_always yield_value(InternalYieldType inSomeValue)
        {
            myYieldValue = inSomeValue;
            myHasYieldValue = true;
            return {};
        }


        InternalYieldType myYieldValue;
        bool myHasYieldValue = false;
    };

    explicit Coroutine(promise_type::CoroutineHandle inCoroutine) : myCoroutineHandle(inCoroutine) { }
    Coroutine(const Coroutine&) = delete;
    Coroutine& operator=(const Coroutine&) = delete;
    Coroutine(Coroutine&& inCoroutine) noexcept : myCoroutineHandle(inCoroutine.myCoroutineHandle)
    {
        inCoroutine.myCoroutineHandle = {};
    }
    
    Coroutine& operator=(Coroutine&& inCoroutine) noexcept
    {
        if(this == &inCoroutine)
            return *this;

        if(myCoroutineHandle)
            myCoroutineHandle.destroy();
        
        myCoroutineHandle = inCoroutine.myCoroutineHandle;
        inCoroutine.myCoroutineHandle = {};
        return *this;
    }

    ~Coroutine()
    {
        if(myCoroutineHandle && myCoroutineHandle.done())
            myCoroutineHandle.destroy();
    }

    bool IsDone() const
    {
        return myCoroutineHandle.done();
    }
    
    void Resume()
    {
        myCoroutineHandle.resume();
    }

    bool HasYieldValue() const
    {
        return myCoroutineHandle.promise().myHasYieldValue;
    }
    
    const InternalYieldType& ConsumeYieldValue() const
    {
        const InternalYieldType& yieldValue = myCoroutineHandle.promise().myYieldValue;
        myCoroutineHandle.promise().myHasYieldValue = false;
        return yieldValue;
    }
    
    bool IsVoidYieldType() const
    {
        return std::is_void<YieldType>();
    }

    bool HasReturnValue() const
    {
        return myCoroutineHandle.promise().myHasReturnValue;
    }
    
    const InternalReturnType& ConsumeReturnValue() const
    {
        const InternalReturnType& returnValue = myCoroutineHandle.promise().myReturnValue;
        myCoroutineHandle.promise().myHasReturnValue = false;
        return returnValue;
    }

    bool IsVoidReturnType() const
    {
        return std::is_void<ReturnType>();
    }
    
private:
    promise_type::CoroutineHandle myCoroutineHandle;
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
