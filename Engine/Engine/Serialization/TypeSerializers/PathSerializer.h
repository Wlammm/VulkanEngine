#pragma once
#include "TypeSerializer.h"

class PathSerializer : public TypeSerializer
{
public:
    bool SerializesType(const Class* inClass) const override;

    void Serialize(void* inInstance, const Class* inClass, BinarySerializer* inSerializer) override;
};
