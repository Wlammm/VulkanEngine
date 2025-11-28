#pragma once
#include "Engine/Delegates/Delegate.hpp"

class Type;

class IUniquePtr
{
public:
    virtual void* GetVoidPtr() const = 0;
    virtual const std::string& GetConcreteFullName() const = 0;
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
        : myObject(inOther.myObject), myDeleter(std::move(inOther.myDeleter)), myConcreteTypeFullName(std::move(inOther.myConcreteTypeFullName))
    {
        inOther.Release();
    }
    
    UniquePtr(UniquePtr&& inOther)
    {
        myObject = inOther.myObject;
        myDeleter = inOther.myDeleter;
        myConcreteTypeFullName = inOther.myConcreteTypeFullName;
        inOther.Release();
    }

    UniquePtr& operator=(UniquePtr&& inOther)
    {
        if (this != &inOther)
        {
            Reset();
            myObject = inOther.myObject;
            myDeleter = inOther.myDeleter;
            myConcreteTypeFullName = inOther.myConcreteTypeFullName;
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

    const std::string& GetConcreteFullName() const override
    {
        return myConcreteTypeFullName;
    }
    
    void* GetVoidPtr() const override
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
    std::string myConcreteTypeFullName = "";
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
    ptr.myConcreteTypeFullName = typeid(PtrType).name();
    return ptr;
}

template <typename T>
struct InnerUniquePtrType { using Type = void; }; // fallback

template <typename T>
struct InnerUniquePtrType<UniquePtr<T>> { using Type = T; };

template <typename T>
using InnerUniquePtrType_t = typename InnerUniquePtrType<T>::Type;