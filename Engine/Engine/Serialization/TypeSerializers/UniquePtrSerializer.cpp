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

    if (inSerializer->IsReading())
    {
        // Create object instance somehow here...
        
    }
    
    inSerializer->SerializeType(uniquePtr->GetVoidPtr(), typeTemplateArgument.myType, typeTemplateArgument.myIsPointer);
}
