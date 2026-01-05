#pragma once
#include "Vertex.hpp"
#include "Engine/System/System.h"
#include "Engine/Vulkan/ResizableBuffer.h"
#include "Engine/Shaders/Shared/MeshStructs.hpp"

class VertexBufferHandle;
class VulkanBuffer;

/*
 *  This systems handles all vertex buffers for the game. It works by storing all vertex buffers in one big vertex buffer and then using offsets and count when rendering.
 *  It also has another sparse buffer which keeps track of this data. This buffer can then update the offsets in that buffer whenever we remove a vertex buffer and copy
 *  move the vertices in the big buffer. The data are set on a specific index which is why its a sparse buffer.
 */
class VertexBufferSystem : public System
{
public:
    VertexBufferSystem();
    ~VertexBufferSystem();
    
    VertexBufferHandle* UploadVertexBuffer(VulkanBuffer* inStagingBuffer, const uint inVertexCount);
    VertexBufferHandle* UploadVertexBuffer(const List<Vertex>& inVertices);
    void RemoveVertexBuffer(const VertexBufferHandle* inBuffer);

    uint GetUsedBufferSize() const;

    uint GetVertexOffsetFromVertexHandle(VertexBufferHandle* inBuffer) const;
    
private:
    uint myUsedBufferSize = 0;
    uint myCurrentVertexOffset = 0;

    List<VertexBufferHandle*> myVertexBuffers;
    ResizableBuffer* myBuffer = nullptr;
    
    // This buffer contains the offsets & sizes for different meshes. This is a sparse buffer.
    ResizableBuffer* mySparseVertexDataBuffer;
    List<VertexBufferData> mySparseVertexData_CPURepresentation;
    List<uint> myFreeSparseIndices;
};
