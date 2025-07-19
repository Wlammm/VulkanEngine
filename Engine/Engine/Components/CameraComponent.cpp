#include "EnginePch.h"
#include "CameraComponent.h"

#include "TransformComponent.h"
#include "Engine/Engine.h"
#include "Engine/Rendering/RenderSystem.h"
#include "Engine/World/World.h"

class RenderSystem;

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

bool CameraComponent::ScreenToWorldPos(const glm::vec2& inNormalizedScreenPos, glm::vec3& outWorldPos) const
{
    float depth = Engine::GetEngineSystem<RenderSystem>().ReadDepthAtScreenPos(inNormalizedScreenPos);

    if (depth <= 0 || depth >= 1)
        return false;
    
    float ndcX = inNormalizedScreenPos.x * 2.0f - 1.0f;
    float ndcY = inNormalizedScreenPos.y * 2.0f - 1.0f;
    
    glm::mat4 view = glm::inverse(GetTransform()->GetMatrix());
    glm::mat4 invVP = glm::inverse(myProjection * view);
    
    glm::vec4 clipSpace = glm::vec4(ndcX, ndcY, depth, 1.0f);
    glm::vec4 worldSpace = invVP * clipSpace;
    worldSpace /= worldSpace.w;
    outWorldPos = glm::vec3(worldSpace);
    
    return true;
}

glm::vec3 CameraComponent::ScreenToNearPlane(const glm::vec2& inNormalizedScreenPos) const
{
    float ndcX = inNormalizedScreenPos.x * 2.0f - 1.0f;
    float ndcY = inNormalizedScreenPos.y * 2.0f - 1.0f;

    glm::vec4 clipSpace = glm::vec4(ndcX, ndcY, 0, 1.0f);

    glm::mat4 view = glm::inverse(GetTransform()->GetMatrix());
    glm::mat4 invVP = glm::inverse(myProjection * view);

    glm::vec4 worldSpace = invVP * clipSpace;
    worldSpace /= worldSpace.w;

    return glm::vec3(worldSpace);
}

glm::vec3 CameraComponent::ScreenToFarPlane(const glm::vec2& inNormalizedScreenPos) const
{
    float ndcX = inNormalizedScreenPos.x * 2.0f - 1.0f;
    float ndcY = inNormalizedScreenPos.y * 2.0f - 1.0f;

    glm::vec4 clipSpace = glm::vec4(ndcX, ndcY, 1.0f, 1.0f);

    glm::mat4 view = glm::inverse(GetTransform()->GetMatrix());
    glm::mat4 invVP = glm::inverse(myProjection * view);

    glm::vec4 worldSpace = invVP * clipSpace;
    worldSpace /= worldSpace.w;

    return glm::vec3(worldSpace);
}

glm::vec3 CameraComponent::GetRayDirectionFromScreen(const glm::vec2& inNormalizedScreenPos) const
{
    float ndcX = inNormalizedScreenPos.x * 2.0f - 1.0f;
    float ndcY = inNormalizedScreenPos.y * 2.0f - 1.0f;

    glm::vec4 clipNear = glm::vec4(ndcX, ndcY, 0.0f, 1.0f);
    glm::vec4 clipFar  = glm::vec4(ndcX, ndcY, 1.0f, 1.0f);

    glm::mat4 view = glm::inverse(GetTransform()->GetMatrix());
    glm::mat4 invVP = glm::inverse(myProjection * view);

    glm::vec4 worldNear = invVP * clipNear;
    glm::vec4 worldFar  = invVP * clipFar;

    worldNear /= worldNear.w;
    worldFar  /= worldFar.w;

    glm::vec3 direction = glm::normalize(glm::vec3(worldFar - worldNear));
    return direction;
}

float CameraComponent::ToLinearDepth(const float inNonLinearDepth) const
{
    return (2.0f * myNearPlane * myFarPlane) / (myFarPlane + myNearPlane - inNonLinearDepth * (myFarPlane - myNearPlane));
}

float CameraComponent::ToNormalizedLinearDepth(const float inNDCDepth) const
{
    return (ToLinearDepth(inNDCDepth) - myNearPlane) / (myFarPlane - myNearPlane);
}
