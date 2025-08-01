#include "EnginePch.h"
#include "CharacterControllerComponent.h"

#include <characterkinematic/PxCapsuleController.h>
#include <characterkinematic/PxControllerManager.h>

#include <algorithm>
#include <PxRigidDynamic.h>

#include "TransformComponent.h"
#include "Engine/Core/Time.h"
#include "Engine/Physics/PhysicsListener.h"
#include "Engine/Physics/PhysicsSystem.h"
#include "Engine/World/World.h"

void CharacterControllerComponent::OnCreate()
{
    Component::OnCreate();

    GetTransform().OnPositionChanged.Bind(&CharacterControllerComponent::OnPositionChanged, this);
    
    PhysicsSystem& physicsSystem = GetWorld()->GetWorldSystem<PhysicsSystem>();
    
    TransformComponent& transform = GetTransform();
    const glm::vec3 position = transform.GetPosition();
    
    physx::PxCapsuleControllerDesc desc;
    desc.setToDefault();
    desc.position = {position.x, position.y, position.z};
    desc.height = myHeight;
    desc.radius = myRadius;
    desc.slopeLimit = glm::radians(mySlopeLimitDegrees);
    desc.material = physicsSystem.GetDefaultMaterial();
    desc.reportCallback = physicsSystem.GetPhysicsListener();
    desc.behaviorCallback = physicsSystem.GetPhysicsListener();
    desc.nonWalkableMode = physx::PxControllerNonWalkableMode::ePREVENT_CLIMBING_AND_FORCE_SLIDING;
    desc.density = 20;
    desc.scaleCoeff = 0.8f;
    desc.contactOffset = 0.1f;
    desc.stepOffset = myStepOffset;

    myController = physicsSystem.GetControllerManager()->createController(desc);
    myController->getActor()->userData = GetActor();
    myController->setUserData(GetActor());
}

void CharacterControllerComponent::OnDestroy()
{
    Component::OnDestroy();
    
    GetTransform().OnPositionChanged.UnBind(&CharacterControllerComponent::OnPositionChanged, this);
    myController->release();
    myController = nullptr;
}

void CharacterControllerComponent::OnPositionChanged()
{
    const glm::vec3 position = GetTransform().GetPosition() + myPositionOffset;
    myController->setFootPosition({position.x, position.y, position.z});
}

void CharacterControllerComponent::Move(const glm::vec3& inMovement)
{
    myDisplacement += inMovement;
}

void CharacterControllerComponent::TickPhysics()
{
    Component::TickPhysics();

    if (!myController)
        return;

    const physx::PxExtendedVec3 footPosition = myController->getFootPosition();
    
    if (!GetTransform().IsPositionDirty())
        GetTransform().SetPosition(physx::PxVec3{static_cast<float>(footPosition.x) - myPositionOffset.x, static_cast<float>(footPosition.y) - myPositionOffset.y, static_cast<float>(footPosition.z) - myPositionOffset.z });

    if (myUseGravity)
    {
        myDownVelocity += myGravity * Time::GetDeltaTime();
        myDownVelocity = std::min(myDownVelocity, myTerminalVelocity);

        if (IsGrounded() && myDownVelocity > 0)
            myDownVelocity = myGravity * 0.01f;
    }

    myDisplacement += -glm::up() * myDownVelocity * Time::GetDeltaTime();

    myCollisionFlags = myController->move({myDisplacement.x, myDisplacement.y, myDisplacement.z}, myMinDist, Time::GetDeltaTime(), myFilter);
    myDisplacement = glm::vec3(0, 0, 0);
    
}

float CharacterControllerComponent::GetHeight() const
{
    return myHeight;
}

void CharacterControllerComponent::SetHeight(const float inHeight)
{
    if (!myController)
        return;

    myHeight = std::max(inHeight, 0.05f);
    myController->resize(myHeight);
}

float CharacterControllerComponent::GetRadius() const
{
    return myRadius;
}

void CharacterControllerComponent::SetRadius(const float inRadius)
{
    if (!myController)
        return;

    myRadius = std::max(inRadius, 0.05f);
    OnDestroy();
    OnCreate();
}

bool CharacterControllerComponent::IsGrounded() const
{
    return myCollisionFlags & physx::PxControllerCollisionFlag::eCOLLISION_DOWN;
}

void CharacterControllerComponent::SetPositionOffset(const glm::vec3& inOffset)
{
    myPositionOffset = inOffset;
}

void CharacterControllerComponent::Jump(const float inJumpForce)
{
    myDownVelocity = -1 * inJumpForce;
}