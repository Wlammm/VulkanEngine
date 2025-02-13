#include "EnginePch.h"
#include "ComponentSystem.h"
#include "GameObject.h"
#include "Components/TransformComponent.h"

void ComponentSystem::Tick()
{
    ZoneScoped;
    for(GameObject* gameObject : myObjects)
    {
        gameObject->Tick();
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
    for(GameObject* gameObject : myObjects)
    {
        gameObject->TickPhysics();
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