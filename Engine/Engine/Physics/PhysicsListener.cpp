#include "EnginePch.h"
#include "PhysicsListener.h"

#include <PxActor.h>
#include <PxRigidDynamic.h>
#include <extensions/PxRigidBodyExt.h>

#include "Engine/Engine.h"
#include "Engine/ComponentSystem/GameObject.h"
#include "Engine/World/World.h"

void PhysicsListener::onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count)
{
    for(uint i = 0; i < count; ++i)
    {
        const physx::PxTriggerPair& current = pairs[i];

        GameObject* firstObject = (GameObject*)current.triggerActor->userData;
        GameObject* secondObject = (GameObject*)current.otherActor->userData;

        if(myCollisionPairs.Contains({firstObject, secondObject}))
        {
            myDequeueCollisionsDelegate.Bind([firstObject, secondObject]()
            {
                Engine::GetWorld().GetComponentSystem().OnTriggerExitForGameObject(*firstObject, *secondObject);
                Engine::GetWorld().GetComponentSystem().OnTriggerExitForGameObject(*secondObject, *firstObject);
            });
            myTriggerPairs.Remove({firstObject, secondObject});
        }
        else
        {
            myDequeueCollisionsDelegate.Bind([firstObject, secondObject]()
            {
                Engine::GetWorld().GetComponentSystem().OnTriggerEnterForGameObject(*firstObject, *secondObject);
                Engine::GetWorld().GetComponentSystem().OnTriggerEnterForGameObject(*secondObject, *firstObject);
            });
            myTriggerPairs.Add({firstObject, secondObject});
        }
    }
}

void PhysicsListener::onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs)
{
    GameObject* firstObject = (GameObject*)pairHeader.actors[0]->userData;
    GameObject* secondObject = (GameObject*)pairHeader.actors[1]->userData;

    if(myCollisionPairs.Contains({firstObject, secondObject}))
    {
        myDequeueCollisionsDelegate.Bind([firstObject, secondObject]()
        {
            Engine::GetWorld().GetComponentSystem().OnCollisionExitForGameObject(*firstObject, *secondObject);
            Engine::GetWorld().GetComponentSystem().OnCollisionExitForGameObject(*secondObject, *firstObject);
        });
        myCollisionPairs.Remove({firstObject, secondObject});
    }
    else
    {
        myDequeueCollisionsDelegate.Bind([firstObject, secondObject]()
        {
            Engine::GetWorld().GetComponentSystem().OnCollisionEnterForGameObject(*firstObject, *secondObject);
            Engine::GetWorld().GetComponentSystem().OnCollisionEnterForGameObject(*secondObject, *firstObject);
        });
        myCollisionPairs.Add({firstObject, secondObject});
    }
}

void PhysicsListener::onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count)
{
}

void PhysicsListener::onWake(physx::PxActor** actors, physx::PxU32 count)
{
}

void PhysicsListener::onSleep(physx::PxActor** actors, physx::PxU32 count)
{
}

void PhysicsListener::onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer,
    const physx::PxU32 count)
{
}

void PhysicsListener::onShapeHit(const physx::PxControllerShapeHit& hit)
{
    if (auto* actor = hit.actor->is<physx::PxRigidDynamic>())
    {
        if (hit.dir.y == 0.0f)
        {
            physx::PxReal coeff = actor->getMass() * hit.length * 10;
            physx::PxRigidBodyExt::addForceAtPos(*actor, hit.dir * coeff, { (float)hit.worldPos.x, (float)hit.worldPos.y, (float)hit.worldPos.z }, physx::PxForceMode::eIMPULSE);
        }
    }
}

void PhysicsListener::onControllerHit(const physx::PxControllersHit& hit)
{
    
}

void PhysicsListener::onObstacleHit(const physx::PxControllerObstacleHit& hit)
{
    
}

physx::PxControllerBehaviorFlags PhysicsListener::getBehaviorFlags(const physx::PxShape& shape,
                                                                   const physx::PxActor& actor)
{
    return physx::PxControllerBehaviorFlag::eCCT_SLIDE;
}

physx::PxControllerBehaviorFlags PhysicsListener::getBehaviorFlags(const physx::PxController& controller)
{
    return physx::PxControllerBehaviorFlag::eCCT_SLIDE;
}

physx::PxControllerBehaviorFlags PhysicsListener::getBehaviorFlags(const physx::PxObstacle& obstacle)
{
    return physx::PxControllerBehaviorFlag::eCCT_SLIDE;
}

bool PhysicsListener::filter(const physx::PxController& a, const physx::PxController& b)
{
    return true;
}

void PhysicsListener::Tick()
{
    ZoneScoped;
    myDequeueCollisionsDelegate();
    myDequeueCollisionsDelegate.Clear();
    
    for(const ContactPair& pair : myCollisionPairs)
    {
        Engine::GetWorld().GetComponentSystem().OnCollisionForGameObject(*pair.myFirst, *pair.mySecond);
        Engine::GetWorld().GetComponentSystem().OnCollisionForGameObject(*pair.mySecond, *pair.myFirst);
    }

    for(const ContactPair& pair : myTriggerPairs)
    {
        Engine::GetWorld().GetComponentSystem().OnTriggerForGameObject(*pair.myFirst, *pair.mySecond);
        Engine::GetWorld().GetComponentSystem().OnTriggerForGameObject(*pair.mySecond, *pair.myFirst);
    }
}
