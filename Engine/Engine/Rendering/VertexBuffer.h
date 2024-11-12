#pragma once

// Create using VertexBufferSystem
class VertexBuffer
{
public:
    uint GetOffset() const;
    uint GetVertexCount() const;  
    
private:
    friend class VertexBufferSystem;
    uint myIndex = -1;
};
