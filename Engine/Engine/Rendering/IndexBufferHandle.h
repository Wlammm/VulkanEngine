#pragma once

// Create using IndexBufferSystem
class IndexBufferHandle
{
public:
    uint GetIndex() const;

private:
    friend class IndexBufferSystem;
    uint myIndex = -1;  
};
