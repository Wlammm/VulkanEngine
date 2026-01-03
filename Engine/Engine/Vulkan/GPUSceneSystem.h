#pragma once
#include "Containers/GPUSparseDenseBuffer.h"
#include "Engine/Shaders/Shared/MeshStructs.hpp"
#include "Engine/System/System.h"


class Material;
class Mesh;
class ResizableBuffer;

using MeshInstanceIndex = uint;

class GPUSceneSystem final : public System
{
public:
    GPUSceneSystem();
    
    MeshInstanceIndex AddMeshInstance(const MeshInstanceData& inMeshInstanceData);
    void UpdateMeshInstance(const MeshInstanceIndex inMeshIndex, const MeshInstanceData& inMeshInstanceData);
    void RemoveMeshInstance(const MeshInstanceIndex inMeshInstance);

    uint GetNumObjects() const;
    
    const IGPUList* GetBuffer() const;
    
private:
    GPUSparseDenseBuffer<MeshInstanceData> myMeshes;
};
