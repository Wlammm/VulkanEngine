#include "EditorPch.h"
#include "SelectionSystem.h"

const List<GameObject>& SelectionSystem::GetSelectedObjects()
{
    return mySelectedObjects;
}

bool SelectionSystem::IsObjectSelected(const GameObject& inObject)
{
    return mySelectedObjects.Contains(inObject);
}

void SelectionSystem::SelectObject(const GameObject& inObject)
{
    mySelectedObjects.Add(inObject);
    OnSelectionChanged();
}

void SelectionSystem::DeselectObject(const GameObject& inObject)
{
    if (mySelectedObjects.Contains(inObject))
        mySelectedObjects.Remove(inObject);
}

void SelectionSystem::ClearSelection()
{
    mySelectedObjects.Clear();
    OnSelectionChanged();
}
