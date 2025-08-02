#pragma once
#include "TypeSerializer.h"

class UniquePtrSerializer : public TypeSerializer
{
public:
    bool SerializesType(const Type* inType) const override;
    void Serialize(void* inInstance, const Type* inType, BinarySerializer* inSerializer) override;
};
