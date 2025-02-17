#pragma once
#include "ComponentArray.h"
#include "Engine/System/WorldSystem.h"

class Component;
class GameObject;

class ComponentSystem : public WorldSystem
{
public:
    ComponentSystem(World* inWorld) : WorldSystem(inWorld) {}

    void Tick() override;
    void TickPhysics();

    GameObject* CreateGameObject();
    void DestroyGameObject(GameObject* inGameObject);

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
    const SegmentedList<ComponentType, ComponentArray<ComponentType>::ComponentsPerSegment>& GetAllComponentsOfType() const
    {
        return GetComponentArrayForType<ComponentType>().GetAllComponents();
    }

    const List<IComponentArray*>& GetAllComponentArrays() const;
    
private:
    List<GameObject*> myObjects{};
    List<GameObject*> myObjectsToDestory{};

    mutable List<IComponentArray*> myComponentArrays;
};
