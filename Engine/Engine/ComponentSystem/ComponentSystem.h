#pragma once
#include "ComponentArray.h"
#include "GameObjectID.hpp"
#include "GameObjectTag.hpp"
#include "Engine/Delegates/MulticastDelegate.hpp"
#include "Engine/System/WorldSystem.h"

class TransformComponent;
class ComponentSystem;
class Component;
class GameObject;

struct GameObjectData
{
    std::string myName;

    // Called after a component was added. Right after Component::OnCreate
    MulticastDelegate<void(Component*)> myOnComponentAdded;
    
    // Called right before a component is deleted.
    MulticastDelegate<void(Component*)> myOnComponentRemoved;

    ComponentSystem* myComponentSystem = nullptr;
    TransformComponent* myTransform = nullptr;
    
    bool myRenderStateDirty = false;
    bool myPhysicsStateDirty = false;

    META(SerializeField)
    TagMask myTags;
};

class ComponentSystem : public WorldSystem
{
public:
    ComponentSystem(World* inWorld) : WorldSystem(inWorld) {}
    ~ComponentSystem();

    void Tick() override;
    void EditorTick() override;
    void TickPhysics();

    void OnCollisionEnterForGameObject(const GameObject& inObject, const GameObject& inOther);
    void OnCollisionForGameObject(const GameObject& inObject, const GameObject& inOther);
    void OnCollisionExitForGameObject(const GameObject& inObject, const GameObject& inOther);
    
    void OnTriggerEnterForGameObject(const GameObject& inObject, const GameObject& inOther);
    void OnTriggerForGameObject(const GameObject& inObject, const GameObject& inOther);
    void OnTriggerExitForGameObject(const GameObject& inObject, const GameObject& inOther);

    GameObject CreateGameObject(const std::string& inObjectName);
    void DestroyGameObject(const GameObject& inGameObject);

    const List<GameObjectID>& GetAllGameObjects() const;
    
    template<typename ComponentType>
    ComponentArray<ComponentType>& GetComponentArrayForType() const
    {
        // TODO: Can this be constant look up time? Not sure its even worth spending time on.
        for(IComponentArray* array : myComponentArrays)
        {
            if(ComponentArray<ComponentType>* castedArray = dynamic_cast<ComponentArray<ComponentType>*>(array))
            {
                return *castedArray;
            }
        }

        ComponentArray<ComponentType>* newArray = new ComponentArray<ComponentType>();
        myComponentArrays.Add(newArray);
        return *newArray;
    }
    
    template<typename ComponentType>
    ComponentType* GetAnyComponentOfType()
    {
        ComponentArray<ComponentType>& componentArray = GetComponentArrayForType<ComponentType>();
        return componentArray.GetFirstComponentOrNull();
    }

    template<typename ComponentType>
    const plf::colony<ComponentType>& GetAllComponentsOfType() const
    {
        return GetComponentArrayForType<ComponentType>().GetAllComponents();
    }

    const List<IComponentArray*>& GetAllComponentArrays() const;

    static GameObjectData& GetGameObjectData(const GameObject& inGameObject);

private:
    void TickObjectDeletes();
    
private:
    
    META(SerializeField)
    // We use gameObjectID's here to avoid a circular include with GameObject.h
    List<GameObjectID> myObjects{};
    List<GameObjectID> myObjectsToDestory{};

    inline static std::unordered_map<GameObjectID, GameObjectData> myGameObjectData{};

    META(SerializeField)
    mutable List<IComponentArray*> myComponentArrays;
};
