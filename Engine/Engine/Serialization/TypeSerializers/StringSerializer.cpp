#include "EnginePch.h"
#include "StringSerializer.h"

#include "Engine/Serialization/BinarySerializer.h"

bool StringSerializer::SerializesType(const Class* inClass) const
{
    const Class* stringClass = ReflectionSystem::GetClass<std::string>();
    return stringClass == inClass;
}

void StringSerializer::Serialize(void* inInstance, const Class* inClass, BinarySerializer* inSerializer)
{
    std::string* string = (std::string*)inInstance;

    int size = static_cast<int>(string->size());
    inSerializer->SerializeCopyable(size);

    if (inSerializer->IsReading())
        string->resize(size);

    inSerializer->SerializeBinaryData(string->data(), size);
}