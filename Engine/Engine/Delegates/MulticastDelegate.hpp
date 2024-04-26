#pragma once
#include "Delegate.hpp"

/*
 * A multicast delegate functions like a normal delegate except it can store multiple, which will be invoked when fired.
 * Due to this the delegates can only have void as their return type.
 *
 * KEEP IN MIND THAT THIS CLASS IS NOT THREAD SAFE!!!
 *
 * Usage examples:
 * someMulticastDelegate += someOtherDelegate;
 * someMulticastDelegate += SomeFreeOrStaticFunction;
 * someMulticastDelegate.Bind(&SomeClass::SomeMemberFunctionThatCanBeConst, someInstanceReferenceOrPointer);
 *
 * Unbinding can be done using -= operator or UnBind with the same syntax as above.
*/
template<typename... Args>
class MulticastDelegate
{
public:
    MulticastDelegate() = default;

    void Bind(const Delegate<void(Args...)> inDelegate)
    {
        check(!myBoundDelegates.Contains(inDelegate) && "Delegate is already bound.");
        myBoundDelegates.Add(inDelegate);
    }

    void operator+=(const Delegate<void(Args...)> inDelegate)
    {
        Bind(inDelegate);
    }

    void UnBind(const Delegate<void(Args...)> inDelegate)
    {
        check(myBoundDelegates.Contains(inDelegate) && "Delegate is not bound.");
        myBoundDelegates.Remove(inDelegate);
    }

    void operator-=(const Delegate<void(Args...)> inDelegate)
    {
        UnBind(inDelegate);
    }


    /*
     * Ugly looking syntax but these support binding & unbinding as such:
     * someMulticastDelegate.Bind(&SomeClass::someMemberFunctionThatCanBeConst, someClassReferenceOrPointer);
     */
    void Bind(void(*freeFunction)(Args...))
    {
        Bind(Delegate<void(Args...)>(freeFunction));
    }
    
    template<typename Class>
    void Bind(void(Class::*memberFunction)(Args...), Class* inClass)
    {
        Bind(Delegate<void(Args...)>(memberFunction, inClass));
    }

    template<typename Class>
    void Bind(void(Class::*memberFunction)(Args...) const, Class* inClass)
    {
        Bind(Delegate<void(Args...)>(memberFunction, inClass));
    }

    template<typename Class>
    void Bind(void(Class::*memberFunction)(Args...), Class& inClass)
    {
        Bind(Delegate<void(Args...)>(memberFunction, inClass));
    }

    template<typename Class>
    void Bind(void(Class::*memberFunction)(Args...) const, Class& inClass)
    {
        Bind(Delegate<void(Args...)>(memberFunction, inClass));
    }

    void UnBind(void(*freeFunction)(Args...))
    {
        UnBind(Delegate<void(Args...)>(freeFunction));
    }
    
    template<typename Class>
    void UnBind(void(Class::*memberFunction)(Args...), Class* inClass)
    {
        UnBind(Delegate<void(Args...)>(memberFunction, inClass));
    }

    template<typename Class>
    void UnBind(void(Class::*memberFunction)(Args...) const, Class* inClass)
    {
        UnBind(Delegate<void(Args...)>(memberFunction, inClass));
    }

    template<typename Class>
    void UnBind(void(Class::*memberFunction)(Args...), Class& inClass)
    {
        UnBind(Delegate<void(Args...)>(memberFunction, inClass));
    }

    template<typename Class>
    void UnBind(void(Class::*memberFunction)(Args...) const, Class& inClass)
    {
        UnBind(Delegate<void(Args...)>(memberFunction, inClass));
    }

    void Invoke(Args... inArgs)
    {
        for(Delegate<void(Args...)>& delegate : myBoundDelegates)
        {
            delegate.Invoke(std::forward<Args>(inArgs)...);
        }
    }

    void operator()(Args... inArgs)
    {
        Invoke(std::forward<Args>(inArgs)...);
    }
   
private:
    List<Delegate<void(Args...)>> myBoundDelegates{};
};
