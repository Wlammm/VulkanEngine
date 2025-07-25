#include "EnginePch.h"
#include "ComponentSystem.h"
#include "GameObject.h"
#include "Engine/Components/TransformComponent.h"

ComponentSystem::~ComponentSystem()
{
    myComponentArrays.Clear();
}

void ComponentSystem::Tick()
{
    ZoneScoped;
    for(const UniquePtr<IComponentArray>& array : myComponentArrays)
    {
        array->Tick();
    }

    TickObjectDeletes();
}

void ComponentSystem::EditorTick()
{
    WorldSystem::EditorTick();

    for (const UniquePtr<IComponentArray>& array : myComponentArrays)
    {
        array->EditorTick();
    }

    TickObjectDeletes();
}

void ComponentSystem::TickPhysics()
{
    ZoneScoped;
    for(const UniquePtr<IComponentArray>& array : myComponentArrays)
    {
        array->TickPhysics();
    }
}

void ComponentSystem::OnCollisionEnterForGameObject(const GameObject& inObject, const GameObject& inOther)
{
    for(const UniquePtr<IComponentArray>& array : myComponentArrays)
    {
        array->OnCollisionEnterForGameObject(inObject, inOther);
    }
}

void ComponentSystem::OnCollisionForGameObject(const GameObject& inObject, const GameObject& inOther)
{
    for(const UniquePtr<IComponentArray>& array : myComponentArrays)
    {
        array->OnCollisionForGameObject(inObject, inOther);
    }
}

void ComponentSystem::OnCollisionExitForGameObject(const GameObject& inObject, const GameObject& inOther)
{
    for(const UniquePtr<IComponentArray>& array : myComponentArrays)
    {
        array->OnCollisionExitForGameObject(inObject, inOther);
    }
}

void ComponentSystem::OnTriggerEnterForGameObject(const GameObject& inObject, const GameObject& inOther)
{
    for(const UniquePtr<IComponentArray>& array : myComponentArrays)
    {
        array->OnTriggerEnterForGameObject(inObject, inOther);
    }
}

void ComponentSystem::OnTriggerForGameObject(const GameObject& inObject, const GameObject& inOther)
{
    for(const UniquePtr<IComponentArray>& array : myComponentArrays)
    {
        array->OnTriggerForGameObject(inObject, inOther);
    }
}

void ComponentSystem::OnTriggerExitForGameObject(const GameObject& inObject, const GameObject& inOther)
{
    for(const UniquePtr<IComponentArray>& array : myComponentArrays)
    {
        array->OnTriggerExitForGameObject(inObject, inOther);
    }
}

GameObject ComponentSystem::CreateGameObject(const std::string& inObjectName)
{
    GameObject gameObject = GameObject(UniqueID::GenerateUniqueID());
    GameObjectData& gameObjectData = myGameObjectData.emplace(gameObject, GameObjectData()).first->second;
    gameObjectData.myComponentSystem = this;
#if DEBUG_GAMEOBJECT_NAMES
    gameObjectData.myName = inObjectName;
#endif
    myObjects.Add(gameObject);
    gameObjectData.myTransform = gameObject.AddComponent<TransformComponent>();
    return gameObject;
}

void ComponentSystem::DestroyGameObject(const GameObject& inGameObject)
{
    myObjectsToDestory.Add(inGameObject);
}

const List<GameObjectID>& ComponentSystem::GetAllGameObjects() const
{
    return myObjects;
}

const List<UniquePtr<IComponentArray>>& ComponentSystem::GetAllComponentArrays() const
{
    return myComponentArrays;
}

GameObjectData& ComponentSystem::GetGameObjectData(const GameObject& inGameObject)
{
    return myGameObjectData[inGameObject];
}

void ComponentSystem::TickObjectDeletes()
{
    for(GameObjectID& gameObjectID : myObjectsToDestory)
    {
        // TODO: This might cause performance issues. Consider extracting the methods to a utils class that both can use. 
        GameObject gameObject = GameObject(gameObjectID);
        
        // Remove all components.
        for(Component* comp : gameObject.GetComponents())
        {
            comp->OnDestroy();
        }
    
        const List<UniquePtr<IComponentArray>>& arrays = gameObject.GetComponentSystem()->GetAllComponentArrays();
        for(const UniquePtr<IComponentArray>& array : arrays)
        {
            array->TryRemoveComponentForGameObject(gameObject);
        }

        myGameObjectData.erase(gameObject);
        myObjects.Remove(gameObject);
    }
    myObjectsToDestory.Clear();

    for (GameObjectID& id : myObjects)
    {
        GetGameObjectData(id).myRenderStateDirty = false;
    }
}
