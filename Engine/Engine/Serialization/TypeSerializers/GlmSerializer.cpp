#include "EnginePch.h"
#include "GlmSerializer.h"

#include "Engine/Serialization/BinarySerializer.h"

bool GlmSerializer::SerializesType(const Type* inClass) const
{
    return inClass->IsA<glm::vec2>() || inClass->IsA<glm::vec3>() || inClass->IsA<glm::vec4>() || inClass->IsA<glm::quat>();
}

void GlmSerializer::Serialize(void* inInstance, const Type* inClass, BinarySerializer* inSerializer)
{
    inSerializer->SerializeBinaryData(inInstance, inClass->GetSize());
}
