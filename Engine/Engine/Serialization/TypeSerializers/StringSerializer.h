#pragma once
#include "TypeSerializer.h"

class StringSerializer : public TypeSerializer<std::string>
{
public:
    void Write(void* inInstance, BinaryWriter* inWriter) override;
    void Read(void* inInstance, BinaryReader* inReader) override;
};
