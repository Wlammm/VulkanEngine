#pragma once
#include "Engine/System/System.h"
#include "Engine/Shaders/Shared/MeshStructs.hpp"
#include "Engine/Vulkan/Containers/GPUSparseDenseBuffer.h"

class ResizableBuffer;

using PointLightInstanceIndex = uint;

class PointLightSystem : public System
{
public:
    PointLightSystem();

    PointLightInstanceIndex AddLight(const PointLightData& inPointLightData);

    void UpdateLight(const PointLightInstanceIndex inLightInstance, const PointLightData& inPointLightData);
    
    void RemoveLight(const PointLightInstanceIndex inLightInstance);
    
    uint GetNumPointLights() const;
    
private:
    GPUSparseDenseBuffer<PointLightData>* myPointLightBuffer;
    
};
