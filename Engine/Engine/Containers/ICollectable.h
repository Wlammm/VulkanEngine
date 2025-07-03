#pragma once

class ICollectable
{
public:
    virtual int GetElementSize() const = 0;
    virtual int GetNumElements() const = 0;

    virtual void* GetElementsPointer() const = 0;
};
