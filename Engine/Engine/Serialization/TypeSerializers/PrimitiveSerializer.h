#pragma once
#include "TypeSerializer.h"

class PrimitiveSerializer : public TypeSerializer
{
public:
    bool SerializesType(const Class* inClass) const override;

    void Write(void* inInstance, const Class* inClass, BinaryWriter* inWriter) override;
    void Read(void* inInstance, const Class* inClass, BinaryReader* inReader) override;
};
