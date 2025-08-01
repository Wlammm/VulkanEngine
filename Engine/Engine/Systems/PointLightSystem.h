#pragma once
#include "Engine/System/System.h"

class ResizableBuffer;
class TransformComponent;
class PointLightComponent;
class PointLightSystem : public System
{
public:
    PointLightSystem();
    ~PointLightSystem();

    const ResizableBuffer* GetBuffer() const;
    
    void AddLight(TransformComponent& inTransform, PointLightComponent* inLight);

private:
    ResizableBuffer* myBuffer = nullptr;
    uint myNumPointLights = 0;
};
