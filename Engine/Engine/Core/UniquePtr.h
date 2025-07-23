#pragma once
#include "Engine/Delegates/Delegate.hpp"


template <typename PtrType>
class UniquePtr
{
public:
    UniquePtr()
    {
    }

    ~UniquePtr()
    {
        Reset();
    }

    UniquePtr(UniquePtr&& inOther)
    {
        myObject = inOther.myObject;
        myDeleter = inOther.myDeleter;
    }

    UniquePtr& operator=(UniquePtr&& inOther)
    {
        if (this != &inOther)
        {
            Reset();
            myObject = inOther.myObject;
            myDeleter = inOther.myDeleter;
            inOther.Release();
        }
        return *this;
    }
    
    UniquePtr& operator=(nullptr_t) {
        Reset();
        return *this;
    }

    UniquePtr(const UniquePtr& inOther) = delete;
    UniquePtr& operator=(const UniquePtr& inOther) = delete;

    PtrType* Get() const
    {
        return myObject;
    }

    void Reset()
    {
        if (myDeleter.IsValid())
            myDeleter(myObject);
        myObject = nullptr;
        myDeleter = nullptr;
    }

    void Release()
    {
        myObject = nullptr;
        myDeleter = nullptr;
    }

    operator bool() const
    {
        return myObject != nullptr;
    }

    PtrType* operator->() const
    {
        return Get();
    }


    template<typename PtrType, typename... Args>
    friend UniquePtr<PtrType> MakeUnique(Args&&... inArgs);
    
private:
    PtrType* myObject = nullptr;
    Delegate<void(PtrType* inPtr)> myDeleter = nullptr;
};

template<typename PtrType, typename... Args>
UniquePtr<PtrType> MakeUnique(Args&&... inArgs)
{
    PtrType* object = new PtrType(std::forward<Args>(inArgs)...);

    Delegate<void(PtrType*)> deleter = [](PtrType* inPtr)
    {
        inPtr->~PtrType();
        ::operator delete(inPtr);
    };

    UniquePtr<PtrType> ptr;
    ptr.myObject = object;
    ptr.myDeleter = deleter;
    return ptr;
}