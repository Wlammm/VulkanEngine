#pragma once
#include "Engine/Delegates/Delegate.hpp"

class Class;
class Field;

class ImGuiPropertyDrawer
{
public:
    using DrawerFunc = Delegate<bool(void*)>;

    static void RegisterDrawers();
    
    template<typename T>
    static void RegisterDrawer(const Class* type, Delegate<bool(T&)> inDrawer)
    {
        myDrawers[type] = [inDrawer](void* data) -> bool
        {
            T& val = *reinterpret_cast<T*>(data);
            return inDrawer(val);
        };
    }

    // Returns true if property is changed.
    static bool DrawProperty(const Field& inField, void* inInstance);

private:
    static inline std::unordered_map<const Class*, DrawerFunc> myDrawers;
};
