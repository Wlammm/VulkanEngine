#pragma once
#include "Engine/Delegates/Delegate.hpp"


template <typename Type>
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

    UniquePtr(nullptr_t)
    {
        Reset();
    }

    template<typename OtherType>
    UniquePtr(UniquePtr<OtherType>&& inOther, typename std::enable_if<std::is_convertible<OtherType*, Type*>::value>::type* = nullptr)
        : myObject(inOther.myObject), myDeleter(std::move(inOther.myDeleter))
    {
        inOther.Release();
    }
    
    UniquePtr(UniquePtr&& inOther)
    {
        myObject = inOther.myObject;
        myDeleter = inOther.myDeleter;
        inOther.Release();
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

    bool operator==(Type* inOther) const
    {
        return myObject == inOther;
    }
    
    UniquePtr& operator=(nullptr_t) {
        Reset();
        return *this;
    }

    Type& operator*() const
    {
        return *myObject;
    }

    UniquePtr(const UniquePtr& inOther) = delete;
    UniquePtr& operator=(const UniquePtr& inOther) = delete;

    Type*Get() const
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

    Type* operator->() const
    {
        return Get();
    }

    template<typename PtrType, typename... Args>
    friend UniquePtr<PtrType> MakeUnique(Args&&... inArgs);
    
private:
    template<typename OtherType>
    friend class UniquePtr;
    
    Type* myObject = nullptr;
    Delegate<void(void* inPtr)> myDeleter = nullptr;
};

template<typename PtrType, typename... Args>
UniquePtr<PtrType> MakeUnique(Args&&... inArgs)
{
    PtrType* object = new PtrType(std::forward<Args>(inArgs)...);

    Delegate<void(void*)> deleter = [](void* inPtr)
    {
        PtrType* object = static_cast<PtrType*>(inPtr);
        object->~PtrType();
        ::operator delete(object);
    };

    UniquePtr<PtrType> ptr;
    ptr.myObject = object;
    ptr.myDeleter = deleter;
    return ptr;
}