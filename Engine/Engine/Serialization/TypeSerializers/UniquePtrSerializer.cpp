#include "EnginePch.h"
#include "UniquePtrSerializer.h"

#include "Engine/Serialization/BinarySerializer.h"

bool UniquePtrSerializer::SerializesType(const Type* inType) const
{
    return inType->IsA<IUniquePtr>();
}

void UniquePtrSerializer::Serialize(void* inInstance, const Type* inType, BinarySerializer* inSerializer)
{
    const TypeTemplateArgument& typeTemplateArgument = inType->GetTemplateArguments()[0];

    IUniquePtr* uniquePtr = static_cast<IUniquePtr*>(inInstance);

    const Type* type = nullptr;
    std::string concreteTypeFullName;
    if (inSerializer->IsWriting())
    {
        type = ReflectionSystem::GetTypeByFullName(uniquePtr->GetConcreteFullName());
        concreteTypeFullName = type->GetFullName();
    }
    inSerializer->SerializeString(concreteTypeFullName);

    if (inSerializer->IsReading())
    {
        type = ReflectionSystem::GetTypeByFullName(concreteTypeFullName);
        type->CreateUniquePtr(uniquePtr);
    }
    
    inSerializer->SerializeType(uniquePtr->GetVoidPtr(), type, typeTemplateArgument.myIsPointer);
}
