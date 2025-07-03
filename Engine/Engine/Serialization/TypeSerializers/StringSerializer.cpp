#include "EnginePch.h"
#include "StringSerializer.h"

#include "Engine/Serialization/BinaryReader.h"
#include "Engine/Serialization/BinaryWriter.h"

bool StringSerializer::SerializesType(const Class* inClass) const
{
    const Class* stringClass = Engine::GetReflectionSystem().GetClass<std::string>();
    return stringClass == inClass;
}

void StringSerializer::Write(void* inInstance, const Class* inClass, BinaryWriter* inWriter)
{
    std::string* string = (std::string*)inInstance;

    inWriter->Write(string->size());
    inWriter->Write(string->data(), static_cast<int>(string->size()));
}

void StringSerializer::Read(void* inInstance, const Class* inClass, BinaryReader* inReader)
{
    std::string* string = (std::string*)inInstance;

    std::string::size_type size;
    inReader->Read(size);
    string->resize(size);
    inReader->Read(string->data(), static_cast<int>(size));
}
