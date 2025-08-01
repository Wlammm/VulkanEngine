#pragma once
#include "Engine/Delegates/MulticastDelegate.hpp"

class Actor;

class SelectionSystem
{
public:
    static const List<Actor*>& GetSelectedObjects();

    static bool IsObjectSelected(Actor* inObject);

    static void SelectObject(Actor* inObject);
    static void DeselectObject(Actor* inObject);
    
    static void ClearSelection();

    static inline MulticastDelegate<void()> OnSelectionChanged;
    
private:
    inline static List<Actor*> mySelectedObjects{};
};
