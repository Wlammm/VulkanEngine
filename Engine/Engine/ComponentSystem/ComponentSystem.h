#pragma once
#include "GameObject.h"

class GameObject;

class ComponentSystem
{
public:
    ComponentSystem();

    void Update();

    GameObject* CreateGameObject();
    void DestroyGameObject(GameObject* inGameObject);

    template<typename ComponentType>
    ComponentType* GetAnyComponentOfType() const
    {
        for(GameObject* object : myObjects)
        {
            if(ComponentType* component = object->GetComponent<ComponentType>())
                return component;
        }
        return nullptr;
    }

    template<typename ComponentType>
    List<GameObject*> GetAllGameObjectsWithComponent() const
    {
        List<GameObject*> objects{};
        for(GameObject* object : myObjects)
        {
            if(object->GetComponent<ComponentType>())
                objects.Add(object);
        }
        return objects;
    }
    
    
private:
    List<GameObject*> myObjects{};
    List<GameObject*> myObjectsToDestory{};
};
