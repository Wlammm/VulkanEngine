#include "EnginePch.h"
#include "PhysicsErrorCallback.h"

void PhysicsErrorCallback::reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line)
{
    LOG_ERROR(message);
}
