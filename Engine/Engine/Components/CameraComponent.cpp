#include "EnginePch.h"
#include "CameraComponent.h"

#include "Engine/World/World.h"

CameraComponent::CameraComponent()
{
        
}

const glm::mat4& CameraComponent::GetProjection() const
{
    return myProjection;
}

void CameraComponent::CreateOrthographic(const glm::vec2& inResolution, const float inNearPlane, const float inFarPlane)
{
    myNearPlane = inNearPlane;
    myFarPlane = inFarPlane;
    myResolution = inResolution;
    myProjection = glm::ortho(inResolution.x * -0.5f, inResolution.x * 0.5f, inResolution.y * -0.5f, inResolution.y * 0.5f, inNearPlane, inFarPlane);
    myProjection[1][1] *= -1;
    myIsOrthographic = true;
}

void CameraComponent::CreatePerspective(const glm::vec2& inResolution, const float inFov, const float inNearPlane, const float inFarPlane)
{
    myFov = inFov;
    myNearPlane = inNearPlane;
    myFarPlane = inFarPlane;
    myResolution = inResolution;

    const float aspectRatio = inResolution.x / inResolution.y;
    myProjection = glm::perspective(glm::radians(inFov / aspectRatio), aspectRatio, inNearPlane, inFarPlane);
    myProjection[1][1] *= -1;
    myIsOrthographic = false;
}

void CameraComponent::SetAsMainCamera()
{
    GetWorld()->SetMainCamera(this);
}
