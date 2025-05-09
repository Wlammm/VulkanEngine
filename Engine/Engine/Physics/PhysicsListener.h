#pragma once
#include <PxSimulationEventCallback.h>

#include "Engine/Delegates/MulticastDelegate.hpp"

class GameObject;

class PhysicsListener : public physx::PxSimulationEventCallback
{
public:
    void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) override;
    void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs) override;
    void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count) override;
    void onWake(physx::PxActor** actors, physx::PxU32 count) override;
    void onSleep(physx::PxActor** actors, physx::PxU32 count) override;
    void onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count) override;

    void Tick();
    
private:
    struct ContactPair
    {
        bool operator==(const ContactPair& inOther) const
        {
            return  inOther.myFirst == myFirst && inOther.mySecond == mySecond ||
                    inOther.mySecond == myFirst && inOther.myFirst == mySecond;
        }
        
        GameObject* myFirst = nullptr;
        GameObject* mySecond = nullptr;
    };

    MulticastDelegate<void()> myDequeueCollisionsDelegate;

    List<ContactPair> myCollisionPairs;
    List<ContactPair> myTriggerPairs;
};
