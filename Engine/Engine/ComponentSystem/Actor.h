#pragma once
#include "GameObjectTag.hpp"
#include "Engine/Components/TransformComponent.h"

class Component;
class World;
class Type;

class Actor
{
public:
    Actor();

    void DoOnCreate();
    virtual void OnCreate() {};

    void DoTick();
    virtual void Tick() {}

    void DoEditorTick();

    void TickRenderState();
    
    void DoTickPhysics();
    virtual void TickPhysics() {}

    void DoOnTriggerEnter(Actor* inOther);
    virtual void OnTriggerEnter(Actor* inOther) {}
    void DoOnTrigger(Actor* inOther);
    virtual void OnTrigger(Actor* inOther) {}
    void DoOnTriggerExit(Actor* inOther);
    virtual void OnTriggerExit(Actor* inOther) {}

    void DoOnCollisionEnter(Actor* inOther);
    virtual void OnCollisionEnter(Actor* inOther) {}
    void DoOnCollision(Actor* inOther);
    virtual void OnCollision(Actor* inOther) {}
    void DoOnCollisionExit(Actor* inOther);
    virtual void OnCollisionExit(Actor* inOther) {}
    
    TransformComponent& GetTransform();

    World* GetWorld() const;

    void AddTags(const TagMask inTagMask);
    void RemoveTags(TagMask inTags);
    void ClearTags();
    
    bool HasAllTags(TagMask inTags) const;
    TagMask GetTags() const;
    bool HasAnyTag(const TagMask inTags) const;

    void MarkRenderStateDirty();

    void SetName(const std::string& inName);
    const std::string& GetName() const;

    void Destroy();

    List<Component*> GetAllComponents() const;

    template<typename ComponentType>
    ComponentType* GetComponent()
    {
        for (Component* comp : myComponents)
        {
            if (ComponentType* castedComp = dynamic_cast<ComponentType*>(comp))
            {
                return castedComp;
            }
        }
        return nullptr;
    }
    
private:
    friend class World;
    void RegisterComponents();
    void RegisterComponentsForType(const Type* inType);
    
private:
    TransformComponent myTransform;

    std::string myName = "Default Actor";
    
    // list of all components including those inside derived types.
    List<Component*> myComponents;

    World* myWorld = nullptr;

    TagMask myTagMask = 0;

    bool myRenderStateDirty = false;
};
