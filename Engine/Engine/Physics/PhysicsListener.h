#pragma once
#include <PxSimulationEventCallback.h>
#include <characterkinematic/PxController.h>
#include <characterkinematic/PxControllerBehavior.h>

#include "Engine/Delegates/MulticastDelegate.hpp"

class Actor;
class GameObject;

class PhysicsListener : public physx::PxSimulationEventCallback, public physx::PxUserControllerHitReport, public physx::PxControllerBehaviorCallback, public physx::PxControllerFilterCallback 
{
public:
    void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) override;
    void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs) override;
    void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count) override;
    void onWake(physx::PxActor** actors, physx::PxU32 count) override;
    void onSleep(physx::PxActor** actors, physx::PxU32 count) override;
    void onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count) override;

    // UserControllerHitReport
    void onShapeHit(const physx::PxControllerShapeHit& hit) override;

    void onControllerHit(const physx::PxControllersHit& hit) override;

    void onObstacleHit(const physx::PxControllerObstacleHit& hit) override;
    
    // ControllerBehvaiourCallback
    physx::PxControllerBehaviorFlags getBehaviorFlags(const physx::PxShape& shape, const physx::PxActor& actor) override;

    physx::PxControllerBehaviorFlags getBehaviorFlags(const physx::PxController& controller) override;

    physx::PxControllerBehaviorFlags getBehaviorFlags(const physx::PxObstacle& obstacle) override;

    // ControllerFilterCallback
    bool filter(const physx::PxController& a, const physx::PxController& b) override;

    
    void Tick();
    
private:
    struct ContactPair
    {
        bool operator==(const ContactPair& inOther) const
        {
            return  inOther.myFirst == myFirst && inOther.mySecond == mySecond ||
                    inOther.mySecond == myFirst && inOther.myFirst == mySecond;
        }
        
        Actor* myFirst = nullptr;
        Actor* mySecond = nullptr;
    };

    MulticastDelegate<void()> myDequeueCollisionsDelegate;

    List<ContactPair> myCollisionPairs;
    List<ContactPair> myTriggerPairs;
};
