#pragma once
#include "TypeSerializer.h"

class ListSerializer : public TypeSerializer
{
public:
    bool SerializesType(const Type* inClass) const override;

    void Serialize(void* inInstance, const Type* inClass, BinarySerializer* inSerializer) override;
};
