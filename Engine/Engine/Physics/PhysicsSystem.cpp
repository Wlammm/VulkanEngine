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

#include "Engine.h"
#include "pvdruntime/OmniPvdWriter.h"
#include "pvdruntime/OmniPvdFileWriteStream.h"

#define PhysXRelease(x) if(x) x->release(); x = nullptr

class Something
{
public:
    virtual void TestSomething() {};
};

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

    const bool tryInitOmniPvd = Engine::GetEngineProperties().HasStartupArgument("-omnipvd");
    
    if(tryInitOmniPvd)
    {
        myOmniPvd = PxCreateOmniPvd(*myFoundation);
        if(myOmniPvd)
        {
            OmniPvdWriter* omniWriter = myOmniPvd->getWriter();
            OmniPvdFileWriteStream* omniFileWriteStream = myOmniPvd->getFileWriteStream();
            if(omniWriter && omniFileWriteStream)
            {
                myWithOmniPvd = true;
                omniWriter->setWriteStream(*omniFileWriteStream);
            }
        }
    }
    
    constexpr bool recordMemoryAllocations = true;
    myPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *myFoundation, *myToleranceScale, recordMemoryAllocations, 0, myOmniPvd);
    check(myPhysics && "Could not create physics engine.");

    if(myWithOmniPvd)
    {
        myOmniPvd->getFileWriteStream()->setFileName("Temporary/OmniPvd/LatestRecording.ovd");

        bool success = myOmniPvd->startSampling();
        if(!success)
        {
            LOG_ERROR("OmniPvd was requested but failed to start sampling.");
        }
    }
    
    physx::PxSceneDesc sceneDesc = physx::PxSceneDesc(physx::PxTolerancesScale());
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
    PhysXRelease(myPhysics);
    PhysXRelease(myOmniPvd);
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


