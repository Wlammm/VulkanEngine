#pragma once
#include "Engine/System/System.h"

struct MeshInstanceData;
class Material;
class Mesh;
class ResizableBuffer;

using MeshInstanceIndex = uint;

class GPUSceneSystem final : public System
{
public:
    GPUSceneSystem();
    ~GPUSceneSystem();

    void Tick();
    
    MeshInstanceIndex AddMeshInstance(const MeshInstanceData& inMeshInstanceData);
    void UpdateMeshInstance(const MeshInstanceIndex inMeshIndex, const MeshInstanceData& inMeshInstanceData);
    void RemoveMeshInstance(const MeshInstanceIndex inMeshInstance);

    const ResizableBuffer* GetSparseBuffer() const;
    const ResizableBuffer* GetDenseBuffer() const;
    uint GetNumObjects() const;
    
private:
    /*
     * Sparse dense buffers are used to managed removal of objects from the scene without breaking indices.
     * The sparse buffer keeps the actual ObjectData, it is not guaranteed to be compacted. 
     * The dense buffer is a compacted buffer that just holds indices to the active sparse datas. 
    */
    List<MeshInstanceIndex> myFreeSparseIndices{};
    MeshInstanceIndex myNextFreeSparseIndex = 0;
    ResizableBuffer* mySparseBuffer = nullptr;

    // The amount of objects added/removed/changed this frame. 
    uint myNumGPUObjectUpdatesThisFrame = 0;


    // This holds a cpu representation of the dense buffer. We need this to find what index a specific mesh is in so we can remove it from the gpu scene.
    ResizableBuffer* myDenseBuffer = nullptr;
    List<uint, uint> myDenseBufferCPURepresentation{};
};
