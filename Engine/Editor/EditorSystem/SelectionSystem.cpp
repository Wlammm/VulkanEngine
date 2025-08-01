#include "EditorPch.h"
#include "SelectionSystem.h"

const List<Actor*>& SelectionSystem::GetSelectedObjects()
{
    return mySelectedObjects;
}

bool SelectionSystem::IsObjectSelected(Actor* inObject)
{
    return mySelectedObjects.Contains(inObject);
}

void SelectionSystem::SelectObject(Actor* inObject)
{
    mySelectedObjects.Add(inObject);
    OnSelectionChanged();
}

void SelectionSystem::DeselectObject(Actor* inObject)
{
    if (mySelectedObjects.Contains(inObject))
        mySelectedObjects.Remove(inObject);
}

void SelectionSystem::ClearSelection()
{
    mySelectedObjects.Clear();
    OnSelectionChanged();
}
