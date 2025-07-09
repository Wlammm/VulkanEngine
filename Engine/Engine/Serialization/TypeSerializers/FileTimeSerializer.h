#pragma once
#include "TypeSerializer.h"

class FileTimeSerializer : public TypeSerializer
{
public:
    bool SerializesType(const Type* inClass) const override;

    void Serialize(void* inInstance, const Type* inClass, BinarySerializer* inSerializer) override;

private:
    long long FileTimeToLongLong(const std::filesystem::file_time_type& inFileTime);

    std::filesystem::file_time_type FileTimeFromLongLong(long long inLongLong);
};
