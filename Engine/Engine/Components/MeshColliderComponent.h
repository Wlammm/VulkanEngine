#pragma once
#include "ColliderComponent.h"

class Model;

class MeshColliderComponent : public ColliderComponent
{
public:
    void OnCreate() override;

    void OnScaleChanged() override;

    void SetModel(Model* inModel);
    
private:
    Model* myModel = nullptr;
};
