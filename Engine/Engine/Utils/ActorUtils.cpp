#include "EnginePch.h"
#include "ActorUtils.h"

#include "Engine/ComponentSystem/Actor.h"
#include "Engine/Reflection/ReflectionSystem.h"
#include "Engine/Serialization/BinarySerializer.h"
#include "Engine/World/World.h"

Actor* ActorUtils::DuplicateActor(Actor* inActor)
{
    const Type* actorType = ReflectionSystem::GetType(inActor);
    
    Actor* duplicateActor = inActor->GetWorld()->SpawnActor(actorType, inActor->GetName() + "_Duplicate");
    
    std::ostringstream stream;
    {
        BinarySerializer serializer(stream, BinarySerializer::Mode::Write);
        serializer.SerializeType(inActor, actorType, false);
    }
    {
        BinarySerializer serializer(stream, BinarySerializer::Mode::Read);
        serializer.SerializeType(duplicateActor, actorType, false);
    }
    duplicateActor->SetName(inActor->GetName() + "_Duplicate");
    return duplicateActor;
}
