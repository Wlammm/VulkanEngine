#include "EnginePch.h"
#include "PhysicsListener.h"

#include <PxActor.h>

#include "ComponentSystem/GameObject.h"

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
                firstObject->OnTriggerExit(secondObject);
                secondObject->OnTriggerExit(firstObject);
            });
            myTriggerPairs.Remove({firstObject, secondObject});
        }
        else
        {
            myDequeueCollisionsDelegate.Bind([firstObject, secondObject]()
            {
                firstObject->OnTriggerEnter(secondObject);
                secondObject->OnTriggerEnter(firstObject);
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
            firstObject->OnCollisionExit(secondObject);
            secondObject->OnCollisionExit(firstObject);
        });
        myCollisionPairs.Remove({firstObject, secondObject});
    }
    else
    {
        myDequeueCollisionsDelegate.Bind([firstObject, secondObject]()
        {
            firstObject->OnCollisionEnter(secondObject);
            secondObject->OnCollisionEnter(firstObject);
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

void PhysicsListener::Tick()
{
    myDequeueCollisionsDelegate();
    myDequeueCollisionsDelegate.Clear();
    
    for(const ContactPair& pair : myCollisionPairs)
    {
        pair.myFirst->OnCollision(pair.mySecond);
        pair.mySecond->OnCollision(pair.myFirst);
    }

    for(const ContactPair& pair : myTriggerPairs)
    {
        pair.myFirst->OnTrigger(pair.mySecond);
        pair.mySecond->OnTrigger(pair.myFirst);
    }
}
