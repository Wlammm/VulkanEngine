#include "EnginePch.h"
#include "CollectableSerializer.h"

#include "Engine/Serialization/BinaryWriter.h"

bool CollectableSerializer::SerializesType(const Class* inClass) const
{
    return inClass->IsA<ICollectable>();
}

void CollectableSerializer::Write(void* inInstance, const Class* inClass, BinaryWriter* inWriter)
{
    ICollectable* collectable = (ICollectable*)inInstance;

    inWriter->Write(collectable->GetNumElements());
    inWriter->Write(collectable->GetElementSize());
    inWriter->Write(collectable->GetElementsPointer(), collectable->GetNumElements() * collectable->GetElementSize());
}

void CollectableSerializer::Read(void* inInstance, const Class* inClass, BinaryReader* inReader)
{
    int a= 0;
}
