#pragma once

/*
 * A multicast delegate functions like a normal delegate except it can store multiple functions which will be executed when fired.
 * Due to this the functions in this delegate can only have void as a return type.
*/
template<typename Function>
class MulticastDelegate
{
public:
    MulticastDelegate()
    {
        std::function<void()> func;
    }

    void operator+=(const Function& inFunction)
    {
        myBoundFunctions.Add(inFunction);
    }

    // _STD forward<_Types>(_Args)...
    void operator(Function&& inArgs...)
    {
        for(const auto& function : myBoundFunctions)
        {
            function(std::forward<Function>(inArgs)...);
        }
    }
    
private:
    MutexList<std::function<Function>> myBoundFunctions;
};
