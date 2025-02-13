#include "EnginePch.h"
#include "ComponentSystem.h"
#include "GameObject.h"
#include "Components/TransformComponent.h"

void ComponentSystem::Tick()
{
    ZoneScoped;
    for(IComponentArray* array : myComponentArrays)
    {
        array->Tick();
    }

    for(GameObject* gameObject : myObjectsToDestory)
    {
        myObjects.Remove(gameObject);
        delete gameObject;
    }
    myObjectsToDestory.Clear();
}

void ComponentSystem::TickPhysics()
{
    ZoneScoped;
    for(IComponentArray* array : myComponentArrays)
    {
        array->TickPhysics();
    }
}

GameObject* ComponentSystem::CreateGameObject()
{
    GameObject* gameObject = new GameObject();
    gameObject->myComponentSystem = this;
    myObjects.Add(gameObject);
    gameObject->AddComponent<TransformComponent>();
    return gameObject;
}

void ComponentSystem::DestroyGameObject(GameObject* inGameObject)
{
    myObjectsToDestory.Add(inGameObject);
}

const List<IComponentArray*>& ComponentSystem::GetAllComponentArrays() const
{
    return myComponentArrays;
}
