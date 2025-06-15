#pragma once
#include "Engine/Delegates/Delegate.hpp"

class Class;
class Field;

class ImGuiPropertyDrawer
{
public:
    using DrawerFunc = Delegate<void(void*)>;

    static void RegisterDrawers();
    
    template<typename T>
    static void RegisterDrawer(const Class* type, Delegate<void(T&)> inDrawer)
    {
        myDrawers[type] = [inDrawer](void* data)
        {
            T& val = *reinterpret_cast<T*>(data);
            inDrawer(val);
        };
    }

    static void DrawProperty(const Field& inField, void* inInstance);

private:
    static inline std::unordered_map<const Class*, DrawerFunc> myDrawers;
};
