#pragma once
#include "Engine/Delegates/Delegate.hpp"

class Type;
class Field;

class AdvancedPropertyDrawer
{
public:
    virtual ~AdvancedPropertyDrawer() = default;
    virtual bool DrawsType(const Type* inType) const = 0;
    virtual bool Draw(void* inInstance, const Field& inField) const = 0;
};

class ImGuiPropertyDrawer
{
public:
    using DrawerFunc = Delegate<bool(void*, const Field& inField)>;

    ~ImGuiPropertyDrawer();
    
    static void RegisterDrawers();
    
    template<typename T>
    static void RegisterSimpleDrawer(const Type* type, Delegate<bool(T&, const Field& inField)> inDrawer)
    {
        mySimpleDrawers[type] = [inDrawer](void* data, const Field& inField) -> bool
        {
            T& val = *reinterpret_cast<T*>(data);
            return inDrawer(val, inField);
        };
    }

    // Used when needing drawers that can handle multiple types, such as an asset drawer.
    template<typename DrawerType>
    static void RegisterAdvancedDrawer()
    {
        myAdvancedDrawers.Emplace(new DrawerType());
    }

    // Returns true if property is changed.
    static bool DrawProperty(const Field& inField, void* inInstance);

private:
    static inline std::unordered_map<const Type*, DrawerFunc> mySimpleDrawers;
    static inline List<AdvancedPropertyDrawer*> myAdvancedDrawers;
};
