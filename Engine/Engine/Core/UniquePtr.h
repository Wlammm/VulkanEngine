#pragma once
#include "Engine/Delegates/Delegate.hpp"
#include "Engine/Reflection/ReflectionSystem.h"


class Type;

class IUniquePtr
{
public:
    virtual void* GetVoidPtr() const = 0;
    virtual const Type* GetConcreteType() const = 0;
};

template <typename ClassType>
class UniquePtr : public IUniquePtr
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
    UniquePtr(UniquePtr<OtherType>&& inOther, typename std::enable_if<std::is_convertible<OtherType*, ClassType*>::value>::type* = nullptr)
        : myObject(inOther.myObject), myDeleter(std::move(inOther.myDeleter))
    {
        if (inOther.myGetConcreteTypeDelegate.IsValid())
        {
            myGetConcreteTypeDelegate = [d = std::move(inOther.myGetConcreteTypeDelegate)](ClassType* instance) -> const Type* {
                return d(static_cast<OtherType*>(instance));
            };
        }
        
        inOther.Release();
    }
    
    UniquePtr(UniquePtr&& inOther)
    {
        myObject = inOther.myObject;
        myDeleter = inOther.myDeleter;
        myGetConcreteTypeDelegate = inOther.myGetConcreteTypeDelegate;
        inOther.Release();
    }

    UniquePtr& operator=(UniquePtr&& inOther)
    {
        if (this != &inOther)
        {
            Reset();
            myObject = inOther.myObject;
            myDeleter = inOther.myDeleter;
            myGetConcreteTypeDelegate = inOther.myGetConcreteTypeDelegate;
            inOther.Release();
        }
        return *this;
    }

    bool operator==(ClassType* inOther) const
    {
        return myObject == inOther;
    }
    
    UniquePtr& operator=(nullptr_t) {
        Reset();
        return *this;
    }

    ClassType& operator*() const
    {
        return *myObject;
    }

    UniquePtr(const UniquePtr& inOther) = delete;
    UniquePtr& operator=(const UniquePtr& inOther) = delete;

    ClassType* Get() const
    {
        return myObject;
    }

    void* GetVoidPtr() const override
    {
        return myObject;
    }

    const Type* GetConcreteType() const override
    {
        return myGetConcreteTypeDelegate(myObject);
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
        myGetConcreteTypeDelegate = nullptr;
    }

    operator bool() const
    {
        return myObject != nullptr;
    }

    template<typename T>
    bool IsA() const
    {
        return dynamic_cast<const T*>(myObject) != nullptr;
    }

    ClassType* operator->() const
    {
        return Get();
    }

    template<typename PtrType, typename... Args>
    friend UniquePtr<PtrType> MakeUnique(Args&&... inArgs);
    
private:
    template<typename OtherType>
    friend class UniquePtr;
    
    ClassType* myObject = nullptr;
    Delegate<void(void* inPtr)> myDeleter = nullptr;
    Delegate<const Type*(ClassType* inInstance)> myGetConcreteTypeDelegate = nullptr;
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

    Delegate<const Type*(PtrType* inInstance)> getConcreteTypeDelegate = [](PtrType* inInstance)
    {
       return ReflectionSystem::GetType(inInstance); 
    };

    UniquePtr<PtrType> ptr;
    ptr.myObject = object;
    ptr.myDeleter = deleter;
    ptr.myGetConcreteTypeDelegate = getConcreteTypeDelegate;
    return ptr;
}

template <typename T>
struct InnerUniquePtrType { using Type = void; }; // fallback

template <typename T>
struct InnerUniquePtrType<UniquePtr<T>> { using Type = T; };

template <typename T>
using InnerUniquePtrType_t = typename InnerUniquePtrType<T>::Type;