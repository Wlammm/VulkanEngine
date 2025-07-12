#pragma once
#include "Engine/Delegates/MulticastDelegate.hpp"
#include "Engine/ComponentSystem/GameObject.h"

class GameObject;

class SelectionSystem
{
public:
    static const List<GameObject>& GetSelectedObjects();

    static bool IsObjectSelected(const GameObject& inObject);

    static void SelectObject(const GameObject& inObject);
    static void DeselectObject(const GameObject& inObject);
    
    static void ClearSelection();

    static inline MulticastDelegate<void()> OnSelectionChanged;
    
private:
    inline static List<GameObject> mySelectedObjects{};
};
