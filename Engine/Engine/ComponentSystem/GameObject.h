#pragma once
#include "ComponentArray.h"
#include "ComponentSystem.h"
#include "GameObjectTag.hpp"
#include "GameObjectID.hpp"

class World;
class TransformComponent;
class ComponentSystem;
class Component;

#ifndef DEBUG_GAMEOBJECT_NAMES
#define DEBUG_GAMEOBJECT_NAMES DEBUG
#endif

class GameObject
{
public:
    GameObject() = default;
    GameObject(const GameObjectID& inGameObjectID);
    GameObject(const GameObject& inOther);

    operator GameObjectID() const
    {
        return myGameObjectID;
    }
    
    GameObject& operator=(const GameObject& inOther);
    bool operator==(const GameObject& inOther) const;
    
    bool IsRenderStateDirty() const;

    void ResetRenderStateDirtyFlag();

    void SetName(const std::string& inName);
    const std::string& GetName() const;

    List<Component*> GetComponents() const;
    
    // TODO: Need to find new way of referencing components. We cant reference them via pointers as they move around in memory. Either that or implement some type of segmented list in ComponentArray.
    template<typename ComponentType, typename... Args>
    ComponentType* AddComponent(Args&&... inArgs)
    {
        ComponentArray<ComponentType>& componentArray = GetGameObjectData().myComponentSystem->GetComponentArrayForType<ComponentType>();
        ComponentType* component = componentArray.AddComponentForGameObject(*this, std::forward<Args>(inArgs)...);
        component->myGameObject = *this;

        component->OnCreate();
        GetOnComponentAdded()(component);
        return component;
    }

    template<typename ComponentType>
    ComponentType* GetComponent() const
    {
        static_assert(std::is_base_of<Component, ComponentType>() && "ComponentType is an invalid type. Did you forget to include it?");
        ComponentArray<ComponentType>& componentArray = GetGameObjectData().myComponentSystem->GetComponentArrayForType<ComponentType>();

        // TODO: This is doing double work. Maybe have the gameobject know if the component exists on it or not?
        if(!componentArray.HasComponentForGameObject(*this))
            return nullptr;
        
        return componentArray.GetComponentForGameObject(*this);
    }

    template<typename ComponentType>
    void RemoveComponent()
    {
        static_assert(std::is_base_of<Component, ComponentType>() && "ComponentType is an invalid type. Did you forget to include it?");
        LOG("Remove Component called.");
        ComponentArray<ComponentType>& componentArray = GetGameObjectData().myComponentSystem->GetComponentArrayForType<ComponentType>();

        ComponentType* component = GetComponent<ComponentType>();
        OnComponentRemoved(component);
        component->OnDestroy();
        
        componentArray.RemoveComponentForGameObject(*this);
    }

    TransformComponent* GetTransform() const;
    void Destroy();

    TagMask GetTags() const;
    void AddTags(TagMask inTags);
    void RemoveTags(TagMask inTags);
    void ClearTags();
    bool HasAnyTag(TagMask inTags) const;
    bool HasAllTags(TagMask inTags) const;
    
    void MarkRenderStateDirty();
    void MarkPhysicsStateDirty();

    World* GetWorld() const;

    MulticastDelegate<void(Component*)>& GetOnComponentAdded() const;
    MulticastDelegate<void(Component*)>& GetOnComponentRemoved() const;

private:
    ComponentSystem* GetComponentSystem() const;
    GameObjectData& GetGameObjectData() const;
    
    friend ComponentSystem;

    META(SerializeField)
    GameObjectID myGameObjectID;
};

// Now define GameObject hashing:
namespace std {
    template<>
    struct hash<GameObject> {
        std::size_t operator()(const GameObject& go) const {
            return std::hash<GameObjectID>()(static_cast<GameObjectID>(go));
        }
    };

    // Add const version, as STL containers will often require this:
    template<>
    struct hash<const GameObject> : hash<GameObject> {};
}