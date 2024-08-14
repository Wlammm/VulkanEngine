#include "EnginePch.h"
#include "ComponentSystem.h"
#include "GameObject.h"
#include "Components/TransformComponent.h"

ComponentSystem::ComponentSystem()
{
    
}

void ComponentSystem::Tick()
{
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