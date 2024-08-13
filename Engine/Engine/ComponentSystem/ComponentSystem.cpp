#include "EnginePch.h"
#include "ComponentSystem.h"
#include "GameObject.h"
#include "Components/TransformComponent.h"

ComponentSystem::ComponentSystem()
{
    
}

void ComponentSystem::Update()
{
    for(GameObject* gameObject : myObjects)
    {
        gameObject->Update();
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