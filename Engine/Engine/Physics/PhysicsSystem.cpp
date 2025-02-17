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

physx::PxFilterFlags SimulationFilterShader(
    physx::PxFilterObjectAttributes attributes0,
    physx::PxFilterData filterData0, 
    physx::PxFilterObjectAttributes attributes1,
    physx::PxFilterData filterData1,
    physx::PxPairFlags& pairFlags,
    const void* constantBlock,
    physx::PxU32 constantBlockSize)
{
    return physx::PxFilterFlag::eDEFAULT;
}


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

    myPvd = physx::PxCreatePvd(*myFoundation);
    myPvdTransport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
    bool successfulPvdConnection = myPvd->connect(*myPvdTransport, physx::PxPvdInstrumentationFlag::eALL);
    if(successfulPvdConnection) 
        LOG("PhysicsSystem::PhysicsSystem - Successfully connected to PhysX PVD");
    
    constexpr bool recordMemoryAllocations = true;
    myPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *myFoundation, *myToleranceScale, recordMemoryAllocations, myPvd);
    check(myPhysics && "Could not create physics engine.");

    physx::PxSceneDesc sceneDesc = physx::PxSceneDesc(*myToleranceScale);
    sceneDesc.gravity = physx::PxVec3(0.0f, -981.f, 0.0f);
    sceneDesc.bounceThresholdVelocity = 0.2f * 981.f;

    sceneDesc.cpuDispatcher = physx::PxDefaultCpuDispatcherCreate(4);
    check(sceneDesc.cpuDispatcher && "Could not create CPU dispatcher.");
    
    sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
    //sceneDesc.filterShader = SimulationFilterShader;
    
    myScene = myPhysics->createScene(sceneDesc);

    myDefaultMaterial = myPhysics->createMaterial(0.5f, 0.5f, 0.2f);
}

PhysicsSystem::~PhysicsSystem()
{
    del(myToleranceScale)
    PhysXRelease(myDefaultMaterial);
    PhysXRelease(myScene);
    PhysXRelease(myPhysics);
    
    if(myPvd && myPvd->isConnected())
    {
        myPvdTransport->disconnect();
        myPvd->disconnect();
    }
    PhysXRelease(myPvdTransport);
    PhysXRelease(myPvd);
    PhysXRelease(myFoundation);
    del(myDefaultAllocator);
    del(myDefaultErrorCallback);
}

void PhysicsSystem::Tick()
{
    ZoneScoped;
    // TODO: This should potentially be moved into a fixed update function.
    if(myHasActiveSimulation)
    {
        myScene->fetchResults(true);
        myHasActiveSimulation = false;
    }

    {
        ZoneScopedN("Dequeue PhysicsCommands");
        for(const Delegate<void(physx::PxPhysics* inPhysics, physx::PxScene* inScene)>& physicsCommand : myPhysicsCommands)
        {
            physicsCommand.Invoke(myPhysics, myScene);
        }
        myPhysicsCommands.Clear();
    }
    
    GetWorld()->GetComponentSystem().TickPhysics();
    
    myHasActiveSimulation = true;
    myScene->simulate(Time::GetDeltaTime());
}

void PhysicsSystem::QueuePhysicsCommand(Delegate<void(physx::PxPhysics* inPhysics, physx::PxScene* inScene)> inDelegate)
{
    myPhysicsCommands.Add(inDelegate);
}

physx::PxTolerancesScale* PhysicsSystem::GetToleranceScale() const
{
    return myToleranceScale;
}

physx::PxMaterial* PhysicsSystem::GetDefaultMaterial() const
{
    return myDefaultMaterial;
}

physx::PxScene* PhysicsSystem::GetScene() const
{
    return myScene;
}


