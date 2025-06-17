#pragma once
#include "Engine/Delegates/MulticastDelegate.hpp"

class GameObject;

class SelectionSystem
{
public:
    static const List<GameObject*>& GetSelectedObjects();

    static bool IsObjectSelected(GameObject* inObject);

    static void SelectObject(GameObject* inObject);
    static void DeselectObject(GameObject* inObject);
    
    static void ClearSelection();

    static inline MulticastDelegate<void()> OnSelectionChanged;
    
private:
    inline static List<GameObject*> mySelectedObjects{};
};
