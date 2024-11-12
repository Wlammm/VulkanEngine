#pragma once
#include "Vertex.hpp"
#include "System/System.h"
#include "Vulkan/ResizableBuffer.h"

class VertexBuffer;
class VulkanBuffer;

struct VertexBufferDataBuffer
{
    uint myOffset;
    uint myCount;
};

class VertexBufferSystem : public System
{
public:
    VertexBufferSystem();
    ~VertexBufferSystem();
    
    VertexBuffer* UploadVertexBuffer(VulkanBuffer* inStagingBuffer, const uint inVertexCount);
    VertexBuffer* UploadVertexBuffer(const List<Vertex>& inVertices);
    void RemoveVertexBuffer(const VertexBuffer* inBuffer);

    const ResizableBuffer* GetGlobalVertexBuffer() const;

    uint GetUsedBufferSize() const;
    
private:
    uint myUsedBufferSize = 0;
    uint myCurrentVertexOffset = 0;

    List<VertexBuffer*> myVertexBuffers;

    
    // This buffer contains the offsets & sizes for different meshes. This is a sparse buffer.
    ResizableBuffer* myVertexBufferDataBuffer;
    List<VertexBufferDataBuffer> myVertexBufferDataBuffer_CPURepresentation;
    uint myNextVertexBufferIndex = 0;
    std::queue<uint> myFreeVertexBufferIndices;

    
    ResizableBuffer* myBuffer = nullptr;
};
