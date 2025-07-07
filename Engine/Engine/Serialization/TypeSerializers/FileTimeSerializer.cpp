#include "EnginePch.h"
#include "FileTimeSerializer.h"

#include "Engine/Serialization/BinarySerializer.h"

bool FileTimeSerializer::SerializesType(const Class* inClass) const
{
    return inClass->IsA<std::filesystem::file_time_type>();
}

void FileTimeSerializer::Serialize(void* inInstance, const Class* inClass, BinarySerializer* inSerializer)
{
    std::filesystem::file_time_type* time = (std::filesystem::file_time_type*)inInstance;
    inSerializer->SerializeCopyable(time);
}