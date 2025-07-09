#include "EnginePch.h"
#include "StringSerializer.h"

#include "Engine/Serialization/BinarySerializer.h"

bool StringSerializer::SerializesType(const Type* inClass) const
{
    const Type* stringClass = ReflectionSystem::GetClass<std::string>();
    return stringClass == inClass;
}

void StringSerializer::Serialize(void* inInstance, const Type* inClass, BinarySerializer* inSerializer)
{
    std::string* string = (std::string*)inInstance;

    int size = static_cast<int>(string->size());
    inSerializer->SerializeCopyable(size);

    if (inSerializer->IsReading())
        string->resize(size);

    inSerializer->SerializeBinaryData(string->data(), size);
}

bool WStringSerializer::SerializesType(const Type* inClass) const
{
    return inClass->IsA<std::wstring>();
}

void WStringSerializer::Serialize(void* inInstance, const Type* inClass, BinarySerializer* inSerializer)
{
    std::wstring* string = (std::wstring*)inInstance;

    int size = static_cast<int>(string->size());
    inSerializer->SerializeCopyable(size);

    if (inSerializer->IsReading())
        string->resize(size);

    inSerializer->SerializeBinaryData(string->data(), size * sizeof(wchar_t));
}
