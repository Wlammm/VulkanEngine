#include "EnginePch.h"
#include "PathSerializer.h"

#include "Engine/Serialization/BinarySerializer.h"

bool PathSerializer::SerializesType(const Class* inClass) const
{
    return inClass->IsA<std::filesystem::path>();
}

void PathSerializer::Serialize(void* inInstance, const Class* inClass, BinarySerializer* inSerializer)
{
    std::filesystem::path* path = (std::filesystem::path*)inInstance;

    std::wstring str = path->wstring();
    
    size_t size = path->string().size();
    inSerializer->SerializeCopyable(size);

    if (inSerializer->IsReading())
        str.resize(size);

    inSerializer->SerializeBinaryData(str.data(), static_cast<int>(size) * sizeof(wchar_t));
}


