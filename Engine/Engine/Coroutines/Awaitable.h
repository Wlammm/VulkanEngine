#pragma once
#include <coroutine>

#include "Engine/AssetRegistry/AssetRegistry.h"

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

    template<typename AssetType>
    class WaitForAsset : public Awaitable
    {
    public:
        WaitForAsset(const std::filesystem::path inPath, AssetRegistry* inAssetRegistry, AssetType*& outAsset)
            : myOutAsset{ outAsset }
        {
            myPath = inPath;
            myAssetRegistry = inAssetRegistry;
            myContinueOnMainThread = ThreadUtils::IsOnMainThread();
        }

        void OnAwait(std::coroutine_handle<> inCoroutineHandle) override
        {
            myAssetRegistry->GetAssetAsync<AssetType>(myPath, [inCoroutineHandle, this](AssetType* inAsset)
            {
                myOutAsset = inAsset;
                if(myContinueOnMainThread)
                {
                    inCoroutineHandle.resume();
                }
                else
                {
                    Engine::GetThreadPool().QueueTask([inCoroutineHandle]()
                    {
                        inCoroutineHandle.resume();
                    });
                }
            });
        }

    private:
        std::filesystem::path myPath;
        AssetRegistry* myAssetRegistry = nullptr;
        AssetType*& myOutAsset;
        bool myContinueOnMainThread = false;
    };

}