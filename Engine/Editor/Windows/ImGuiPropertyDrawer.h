#pragma once
#include "Engine/Delegates/Delegate.hpp"

class Class;
class Field;

class ImGuiPropertyDrawer
{
public:
    using DrawerFunc = Delegate<bool(void*, const Field& inField)>;

    static void RegisterDrawers();
    
    template<typename T>
    static void RegisterDrawer(const Class* type, Delegate<bool(T&, const Field& inField)> inDrawer)
    {
        myDrawers[type] = [inDrawer](void* data, const Field& inField) -> bool
        {
            T& val = *reinterpret_cast<T*>(data);
            return inDrawer(val, inField);
        };
    }

    // Returns true if property is changed.
    static bool DrawProperty(const Field& inField, void* inInstance);

private:
    static inline std::unordered_map<const Class*, DrawerFunc> myDrawers;
};
