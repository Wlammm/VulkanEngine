#include "EnginePch.h"
#include "SinWaveMovementComponent.h"

#include "TransformComponent.h"
#include "Core/Time.h"

void SinWaveMovementComponent::Tick()
{
    GetTransform().Move({0, std::sin(Time::GetSeconds()) * 100 * Time::GetDeltaTime(), 0});
}