#include "EditorPch.h"
#include "SelectionSystem.h"

const List<GameObject*>& SelectionSystem::GetSelectedObjects()
{
    return mySelectedObjects;
}

bool SelectionSystem::IsObjectSelected(GameObject* inObject)
{
    return mySelectedObjects.Contains(inObject);
}

void SelectionSystem::SelectObject(GameObject* inObject)
{
    mySelectedObjects.Add(inObject);
    OnSelectionChanged();
}

void SelectionSystem::DeselectObject(GameObject* inObject)
{
    if (mySelectedObjects.Contains(inObject))
        mySelectedObjects.Remove(inObject);
}

void SelectionSystem::ClearSelection()
{
    mySelectedObjects.Clear();
    OnSelectionChanged();
}
