#pragma once
#include <mutex>

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
    
private:
    GPUSparseDenseBuffer<MeshInstanceData>* myMeshes;

    // Serializes Add/Update/Remove of mesh instances. Add/Remove do swap-and-pop over shared sparse
    // mapping arrays; an unsynchronized access aliases instance indices. Currently only called on the
    // main thread, so this is defense-in-depth against future off-main callers.
    std::mutex myMutex;
};
