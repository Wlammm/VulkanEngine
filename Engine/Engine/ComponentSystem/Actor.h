#pragma once
#include "GameObjectTag.hpp"
#include "Engine/Components/TransformComponent.h"

class Component;
class World;
class Type;

class Actor
{
public:
    friend class Editor;
    Actor();

    void DoOnCreate();
    virtual void OnCreate() {};

    void DoOnDestroy();
    virtual void OnDestroy() {};
    
    void DoTick();
    virtual void Tick() {}

    void DoEditorTick();

    void TickRenderState();
    
    void DoTickPhysics();
    virtual void TickPhysics() {}

    // Override this method and return true if you dont want this actor to be saved in serialization.
    virtual bool IsTransientActor() const { return false;}

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
    META(SerializeField)
    TransformComponent myTransform;

    META(SerializeField)
    std::string myName = "Default Actor";
    
    // list of all components including those inside derived types.
    List<Component*> myComponents;

    World* myWorld = nullptr;

    META(SerializeField)
    TagMask myTagMask = 0;

    bool myRenderStateDirty = true;
};
