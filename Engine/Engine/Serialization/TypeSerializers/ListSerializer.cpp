#include "EnginePch.h"
#include "ListSerializer.h"

#include "Engine/Serialization/BinarySerializer.h"

bool ListSerializer::SerializesType(const Class* inClass) const
{
    return inClass->IsA<IList>();
}

void ListSerializer::Serialize(void* inInstance, const Class* inClass, BinarySerializer* inSerializer)
{
    const ClassTemplateArgument& listStorageType = inClass->GetTemplateArguments().First();
    check(!listStorageType.myIsReference && "Cannot serialize references");
    IList* list = (IList*)inInstance;

    
    int size = list->size();    
    inSerializer->SerializeCopyable(size);
    
    int sizeofElement = list->GetSizeofElement();
    inSerializer->SerializeCopyable(sizeofElement);

    bool isCopyable = inClass->IsCopyable();
    inSerializer->SerializeCopyable(isCopyable);

    bool isPointer = listStorageType.myIsPointer;
    inSerializer->SerializeCopyable(isPointer);

    if (inSerializer->IsReading())
        list->Resize(size);
    
    if (inClass->IsCopyable())
    {
        inSerializer->SerializeBinaryData(list->GetData(), list->size() * list->GetSizeofElement());
        return;
    }
    
    for (int i = 0; i < list->size(); ++i)
    {
        check(false);
        // inSerializer->SerializeClass((char*)list->GetData() + (list->GetSizeofElement() * i), inClass->GetTemplateArguments().First().myType);
    }
}