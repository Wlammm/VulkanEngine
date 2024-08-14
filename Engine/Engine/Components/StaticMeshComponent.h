#pragma once
#include "ComponentSystem/Component.h"

class Model;

class StaticMeshComponent : public Component
{
public:
    void Start() override;
    ~StaticMeshComponent();

    void SetModel(Model* inModel);
    Model* GetModel() const;

private:
    void RegisterMeshesToObjectSystem() const;
    
private:
    Model* myModel = nullptr;
};
