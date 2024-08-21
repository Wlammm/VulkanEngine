#pragma once

// Create using IndexBufferSystem
class IndexBuffer
{
public:
    uint GetOffset() const;
    uint GetIndexCount() const;

private:
    friend class IndexBufferSystem;
    
    uint myOffset = (uint)-1;
    uint myIndexCount;
};
