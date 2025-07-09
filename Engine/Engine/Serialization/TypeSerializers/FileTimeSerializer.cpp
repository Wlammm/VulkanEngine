#include "EnginePch.h"
#include "FileTimeSerializer.h"

#include "Engine/Serialization/BinarySerializer.h"

bool FileTimeSerializer::SerializesType(const Type* inClass) const
{
    return inClass->IsA<std::filesystem::file_time_type>();
}

void FileTimeSerializer::Serialize(void* inInstance, const Type* inClass, BinarySerializer* inSerializer)
{
    std::filesystem::file_time_type* time = (std::filesystem::file_time_type*)inInstance;

    long long fileTime = FileTimeToLongLong(*time);
    inSerializer->SerializeCopyable(fileTime);

    *time = FileTimeFromLongLong(fileTime);
}

long long FileTimeSerializer::FileTimeToLongLong(const std::filesystem::file_time_type& inFileTime)
{
    return inFileTime.time_since_epoch().count();
}

std::filesystem::file_time_type FileTimeSerializer::FileTimeFromLongLong(long long inLongLong)
{
    return std::filesystem::file_time_type(std::filesystem::file_time_type::duration(inLongLong));
}