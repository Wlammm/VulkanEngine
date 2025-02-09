#pragma once

// Create using VertexBufferSystem
class VertexBufferHandle
{
public:
    uint GetIndex() const;
    
private:
    friend class VertexBufferSystem;
    uint myIndex = -1;
};
