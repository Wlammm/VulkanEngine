#include "EnginePch.h"
#include "PhysicsSystem.h"
#include "PhysXInclude.h"
#include <omnipvd/PxOmniPvd.h>
//#include "../pvdruntime/include/OmniPvdWriter.h"
//#include "../pvdruntime/include/OmniPvdFileWriteStream.h"
#include "ComponentSystem/ComponentSystem.h"
#include "Core/Time.h"
#include "World/World.h"
#include "Physics/PhysicsErrorCallback.h"
#include <omnipvd/PxOmniPvd.h>

#define PhysXRelease(x) if(x) x->release()

PhysicsSystem::PhysicsSystem(World* inWorld)
    : WorldSystem(inWorld)
{
    // TODO: Maybe these needs to be made static or into some other system as this is currently created once for each world.
    myDefaultAllocator = new physx::PxDefaultAllocator();
    myDefaultErrorCallback = new PhysicsErrorCallback();
    myToleranceScale = new physx::PxTolerancesScale();
    myToleranceScale->length = 100.0f;
    myToleranceScale->speed = 981.f;
    
    myFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, *myDefaultAllocator, *myDefaultErrorCallback);
    check(myFoundation && "Could not create physics foundation.");

    // OLD PVD CODE:
    //myPvd = physx::PxCreatePvd(*myFoundation);
    //myPvdTransport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
    //bool successfulPvdConnection = myPvd->connect(*myPvdTransport, physx::PxPvdInstrumentationFlag::eALL);
    //if(successfulPvdConnection) 
    //    LOG("PhysicsSystem::PhysicsSystem - Successfully connected to PhysX PVD");
//#if PX_ENABLE_OMNI_PVD 
    //physx::PxOmniPvd* omniPvd = physx::PxCreateOmniPvd(*myFoundation);
    
//#endif
    
    constexpr bool recordMemoryAllocations = true;
    myPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *myFoundation, *myToleranceScale, recordMemoryAllocations, myPvd);
    check(myPhysics && "Could not create physics engine.");

    physx::PxSceneDesc sceneDesc = physx::PxSceneDesc(*myToleranceScale);
    sceneDesc.gravity = physx::PxVec3(0.0f, -981.f, 0.0f);

    sceneDesc.cpuDispatcher = physx::PxDefaultCpuDispatcherCreate(2); // 2 threads for simulation
    check(sceneDesc.cpuDispatcher && "Could not create CPU dispatcher.");

    sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
    
    myScene = myPhysics->createScene(sceneDesc);

    myDefaultMaterial = myPhysics->createMaterial(0.5f, 0.5f, 0.2f);
}

PhysicsSystem::~PhysicsSystem()
{
    del(myToleranceScale)
    PhysXRelease(myDefaultMaterial);
    PhysXRelease(myScene);
    PhysXRelease(myPvdTransport);
    if(myPvd)
        myPvd->disconnect();
    PhysXRelease(myPvd);
    PhysXRelease(myPhysics);
    PhysXRelease(myFoundation);
    del(myDefaultAllocator);
    del(myDefaultErrorCallback);
}

void PhysicsSystem::Tick()
{
    // TODO: This should potentially be moved into a fixed update function.
    if(myHasActiveSimulation)
    {
        myScene->fetchResults(true);
        myHasActiveSimulation = false;
    }

    for(auto& physicsCommand : myPhysicsCommands)
    {
        physicsCommand.Invoke(myPhysics, myScene);
    }
    myPhysicsCommands.Clear();
    
    GetWorld()->GetComponentSystem().TickPhysics();
    
    myHasActiveSimulation = true;
    myScene->simulate(Time::GetDeltaTime());
}

void PhysicsSystem::QueuePhysicsCommand(Delegate<void(physx::PxPhysics* inPhysics, physx::PxScene* inScene)> inDelegate)
{
    myPhysicsCommands.Add(inDelegate);
}

physx::PxMaterial* PhysicsSystem::GetDefaultMaterial() const
{
    return myDefaultMaterial;
}

physx::PxScene* PhysicsSystem::GetScene() const
{
    return myScene;
}


