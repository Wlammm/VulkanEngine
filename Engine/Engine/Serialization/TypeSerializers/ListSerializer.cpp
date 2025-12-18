#include "EnginePch.h"
#include "ListSerializer.h"

#include "Engine/Serialization/BinarySerializer.h"

bool ListSerializer::SerializesType(const Type* inClass) const
{
    return inClass->IsA<IList>();
}

void ListSerializer::Serialize(void* inInstance, const Type* inClass, BinarySerializer* inSerializer)
{
    const TypeTemplateArgument& listStorageType = inClass->GetTemplateArguments().First();
    check(!listStorageType.myIsReference && "Cannot serialize references");
    IList* list = static_cast<IList*>(inInstance);

    if (inSerializer->IsReading())
    {
        list->Clear();
    }
    
    int size = list->size();    
    inSerializer->SerializeCopyable(size);
    
    int sizeofElement = list->GetSizeofElement();
    inSerializer->SerializeCopyable(sizeofElement);

    bool isCopyable = listStorageType.myType->IsCopyable();
    inSerializer->SerializeCopyable(isCopyable);

    std::string listStorageTypeName = listStorageType.myType->GetName();
    inSerializer->SerializeString(listStorageTypeName);

    bool isPointer = listStorageType.myIsPointer;
    inSerializer->SerializeCopyable(isPointer);

    if (inSerializer->IsReading())
    {
        list->Resize(size);

        // Placement new all instances in this class whenever we're reading as we might need the vtable to be setup to serialize the data. This is required for lists inside other lists.
        if (!listStorageType.myType->IsCopyable())
        {
            for (int i = 0; i < size; ++i)
            {
                listStorageType.myType->PlacementNew((char*)list->GetData() + listStorageType.myType->GetSize() * i);
            }
        }
    }

    bool isSameType = sizeofElement == list->GetSizeofElement() && isCopyable == listStorageType.myType->IsCopyable();
    
    if (isSameType && isCopyable && !isPointer)
    {
        inSerializer->SerializeBinaryData(list->GetData(), list->size() * list->GetSizeofElement());
        return;
    }
    else if (!isCopyable)
    {
        for (int i = 0; i < list->size(); ++i)
        {
            void* data = (char*)list->GetData() + (list->GetSizeofElement() * i);
            inSerializer->SerializeType(data, listStorageType.myType, listStorageType.myIsPointer);
        }
    }
}