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

void ComponentSystem::OnCollisionEnterForGameObject(GameObject* inObject, GameObject* inOther)
{
    for(IComponentArray* array : myComponentArrays)
    {
        array->OnCollisionEnterForGameObject(inObject, inOther);
    }
}

void ComponentSystem::OnCollisionForGameObject(GameObject* inObject, GameObject* inOther)
{
    for(IComponentArray* array : myComponentArrays)
    {
        array->OnCollisionForGameObject(inObject, inOther);
    }
}

void ComponentSystem::OnCollisionExitForGameObject(GameObject* inObject, GameObject* inOther)
{
    for(IComponentArray* array : myComponentArrays)
    {
        array->OnCollisionExitForGameObject(inObject, inOther);
    }
}

void ComponentSystem::OnTriggerEnterForGameObject(GameObject* inObject, GameObject* inOther)
{
    for(IComponentArray* array : myComponentArrays)
    {
        array->OnTriggerEnterForGameObject(inObject, inOther);
    }
}

void ComponentSystem::OnTriggerForGameObject(GameObject* inObject, GameObject* inOther)
{
    for(IComponentArray* array : myComponentArrays)
    {
        array->OnTriggerForGameObject(inObject, inOther);
    }
}

void ComponentSystem::OnTriggerExitForGameObject(GameObject* inObject, GameObject* inOther)
{
    for(IComponentArray* array : myComponentArrays)
    {
        array->OnTriggerExitForGameObject(inObject, inOther);
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
