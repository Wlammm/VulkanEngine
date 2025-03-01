#pragma once
#include "Delegate.hpp"
#include "Containers/MutexList.hpp"

template<typename>
class MulticastDelegate;

template<typename Type>
concept IsVoid = std::same_as<Type, void>;

/*
 * A multicast delegate functions like a normal delegate except it can store multiple, which will be invoked when fired.
 * Due to this the delegates can only have void as their return type.
 *
 * Usage examples:
 * someMulticastDelegate += someOtherDelegate;
 * someMulticastDelegate += SomeFreeOrStaticFunction;
 * someMulticastDelegate.Bind(&SomeClass::SomeMemberFunctionThatCanBeConst, someInstanceReferenceOrPointer);
 *
 * Unbinding can be done using -= operator or UnBind with the same syntax as above.
*/
template<typename ReturnType, typename... ArgTypes> requires IsVoid<ReturnType>
class MulticastDelegate<ReturnType(ArgTypes...)> 
{
public:
    MulticastDelegate() = default;

    // Move constructor
    MulticastDelegate(MulticastDelegate&& other) noexcept
    {
        if(this == &other)
            return;

        std::scoped_lock lock(myBoundDelegates.GetMutex(), other.myBoundDelegates.GetMutex());
        myBoundDelegates = std::move(other.myBoundDelegates);
    }

    // Move assignment operator
    MulticastDelegate& operator=(MulticastDelegate&& other) noexcept
    {
        if (this != &other) // Check for self-assignment
        {
            myBoundDelegates = std::move(other.myBoundDelegates); // Move the list
        }
        return *this;
    }

    // Delete copy constructor and copy assignment operator
    MulticastDelegate(const MulticastDelegate&) = delete;
    MulticastDelegate& operator=(const MulticastDelegate&) = delete;

    bool IsBound(const Delegate<void(ArgTypes...)>& inDelegate) const
    {
        return myBoundDelegates.Contains(inDelegate);
    }
    
    void Bind(const Delegate<void(ArgTypes...)> inDelegate)
    {
        check(!myBoundDelegates.Contains(inDelegate) && "Delegate is already bound.");
        myBoundDelegates.Add(inDelegate);
    }

    void operator+=(const Delegate<void(ArgTypes...)> inDelegate)
    {
        Bind(inDelegate);
    }

    void Clear()
    {
        myBoundDelegates.Clear();
    }
    
    void UnBind(const Delegate<void(ArgTypes...)> inDelegate)
    {
        check(myBoundDelegates.Contains(inDelegate) && "Delegate is not bound.");
        myBoundDelegates.Remove(inDelegate);
    }

    void operator-=(const Delegate<void(ArgTypes...)> inDelegate)
    {
        UnBind(inDelegate);
    }

    /*
     * Ugly looking syntax but these support binding & unbinding as such:
     * someMulticastDelegate.Bind(&SomeClass::someMemberFunctionThatCanBeConst, someClassReferenceOrPointer);
     */
    void Bind(const MulticastDelegate<ReturnType(ArgTypes...)>& inMulticastDelegate)
    {
        std::scoped_lock lock {myBoundDelegates.GetMutex(), inMulticastDelegate.myBoundDelegates.GetMutex() };
        for(int i = 0; i < inMulticastDelegate.myBoundDelegates.size(); ++i)
        {
            Bind(inMulticastDelegate.myBoundDelegates[i]);
        }
    }
    
    void Bind(ReturnType(*freeFunction)(ArgTypes...))
    {
        Bind(Delegate<ReturnType(ArgTypes...)>(freeFunction));
    }
    
    template<typename Class>
    void Bind(ReturnType(Class::*memberFunction)(ArgTypes...), Class* inClass)
    {
        Bind(Delegate<ReturnType(ArgTypes...)>(memberFunction, inClass));
    }

    template<typename Class>
    void Bind(ReturnType(Class::*memberFunction)(ArgTypes...) const, Class* inClass)
    {
        Bind(Delegate<ReturnType(ArgTypes...)>(memberFunction, inClass));
    }

    template<typename Class>
    void Bind(ReturnType(Class::*memberFunction)(ArgTypes...), Class& inClass)
    {
        Bind(Delegate<ReturnType(ArgTypes...)>(memberFunction, inClass));
    }

    template<typename Class>
    void Bind(ReturnType(Class::*memberFunction)(ArgTypes...) const, Class& inClass)
    {
        Bind(Delegate<ReturnType(ArgTypes...)>(memberFunction, inClass));
    }

    void UnBind(ReturnType(*freeFunction)(ArgTypes...))
    {
        UnBind(Delegate<ReturnType(ArgTypes...)>(freeFunction));
    }
    
    template<typename Class>
    void UnBind(ReturnType(Class::*memberFunction)(ArgTypes...), Class* inClass)
    {
        UnBind(Delegate<ReturnType(ArgTypes...)>(memberFunction, inClass));
    }

    template<typename Class>
    void UnBind(ReturnType(Class::*memberFunction)(ArgTypes...) const, Class* inClass)
    {
        UnBind(Delegate<ReturnType(ArgTypes...)>(memberFunction, inClass));
    }

    template<typename Class>
    void UnBind(ReturnType(Class::*memberFunction)(ArgTypes...), Class& inClass)
    {
        UnBind(Delegate<ReturnType(ArgTypes...)>(memberFunction, inClass));
    }

    template<typename Class>
    void UnBind(ReturnType(Class::*memberFunction)(ArgTypes...) const, Class& inClass)
    {
        UnBind(Delegate<ReturnType(ArgTypes...)>(memberFunction, inClass));
    }

    void Invoke(ArgTypes... inArgs)
    {
        myBoundDelegates.Lock();
        for(int i = 0; i < myBoundDelegates.size(); ++i)
        {
            const Delegate<ReturnType(ArgTypes...)>& delegate = myBoundDelegates[i];
            delegate.Invoke(std::forward<ArgTypes>(inArgs)...);
        }
        myBoundDelegates.Unlock();
    }

    void operator()(ArgTypes... inArgs)
    {
        Invoke(std::forward<ArgTypes>(inArgs)...);
    }
   
private:
    MutexList<Delegate<void(ArgTypes...)>> myBoundDelegates{};
};
