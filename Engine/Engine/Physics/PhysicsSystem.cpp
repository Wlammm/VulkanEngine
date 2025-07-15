#include "EnginePch.h"
#include "PhysicsSystem.h"
#include "PhysXInclude.h"
#include <omnipvd/PxOmniPvd.h>
//#include "../pvdruntime/include/OmniPvdWriter.h"
//#include "../pvdruntime/include/OmniPvdFileWriteStream.h"
#include "Engine/ComponentSystem/ComponentSystem.h"
#include "Engine/Core/Time.h"
#include "Engine/World/World.h"
#include "Engine/Physics/PhysicsErrorCallback.h"
#include <omnipvd/PxOmniPvd.h>

#include "PhysicsListener.h"

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
    /*
    * Adding exclusions:
    * 
    * uint excluded = Tag::Enemy | Tag::Player;
    * myExcludedCollisions.insert(excluded);
    * 
    */
    pairFlags = physx::PxPairFlag::eTRIGGER_DEFAULT;

    //uint filterCombo = filterData0.word0 | filterData1.word0;
    //if (Main::GetPhysicsManager().myExcludedCollisions.count(filterCombo))
    //{
    //    return physx::PxFilterFlag::eSUPPRESS;
    //}

    pairFlags = physx::PxPairFlag::eNOTIFY_TOUCH_FOUND;
    if (physx::PxFilterObjectIsTrigger(attributes0) || physx::PxFilterObjectIsTrigger(attributes1))
    {
        pairFlags |= physx::PxPairFlag::eTRIGGER_DEFAULT;
        return physx::PxFilterFlag::eDEFAULT;
    }

    pairFlags |= physx::PxPairFlag::eSOLVE_CONTACT;
    pairFlags |= physx::PxPairFlag::eDETECT_DISCRETE_CONTACT;
    pairFlags |= physx::PxPairFlag::eDETECT_CCD_CONTACT;

    //pairFlags |= physx::PxPairFlag::eCONTACT_DEFAULT;
    return physx::PxFilterFlag::eDEFAULT;
}


void PhysicsSystem::CreateStaticObjects()
{
    myDefaultAllocator = new physx::PxDefaultAllocator();
    myDefaultErrorCallback = new PhysicsErrorCallback();
    
    myFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, *myDefaultAllocator, *myDefaultErrorCallback);
    check(myFoundation && "Could not create physics foundation.");
    
    myPvd = physx::PxCreatePvd(*myFoundation);
    myPvdTransport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
    bool successfulPvdConnection = myPvd->connect(*myPvdTransport, physx::PxPvdInstrumentationFlag::eALL);
    if(successfulPvdConnection) 
        LOG("PhysicsSystem::PhysicsSystem - Successfully connected to PhysX PVD");
}

void PhysicsSystem::DestoryStaticObjects()
{
    PxCloseExtensions();
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

PhysicsSystem::PhysicsSystem(World* inWorld)
    : WorldSystem(inWorld)
{
    myListener = new PhysicsListener();
    myToleranceScale = new physx::PxTolerancesScale();
    myToleranceScale->length = 100.0f;
    myToleranceScale->speed = 981.f;

    constexpr bool recordMemoryAllocations = true;
    myPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *myFoundation, *myToleranceScale, recordMemoryAllocations, myPvd);
    check(myPhysics && "Could not create physics engine.");

    physx::PxSceneDesc sceneDesc = physx::PxSceneDesc(*myToleranceScale);
    sceneDesc.gravity = physx::PxVec3(0.0f, -981.f, 0.0f);
    sceneDesc.bounceThresholdVelocity = 0.2f * 981.f;
    
    sceneDesc.cpuDispatcher = physx::PxDefaultCpuDispatcherCreate(4);
    check(sceneDesc.cpuDispatcher && "Could not create CPU dispatcher.");
    
    //sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
    sceneDesc.filterShader = SimulationFilterShader;

    sceneDesc.simulationEventCallback = myListener;
    myScene = myPhysics->createScene(sceneDesc);

    myDefaultMaterial = myPhysics->createMaterial(0.5f, 0.5f, 0.2f);

    if (!PxInitExtensions(*myPhysics, myPvd))
    {
        check(false && "Failed to init PhysX extensions.");
    }
    
    myControllerManager = PxCreateControllerManager(*myScene);
}

PhysicsSystem::~PhysicsSystem()
{
    PhysXRelease(myControllerManager);
    PhysXRelease(myScene);
    del(myToleranceScale);
    PhysXRelease(myDefaultMaterial);
    PhysXRelease(myPhysics);
    del(myListener);
}

void PhysicsSystem::Tick()
{
    ZoneScoped;
    // TODO: This should potentially be moved into a fixed update function. Make sure the Listener::Tick() is outside the fixed function call though.
    if(myHasActiveSimulation)
    {
        myScene->fetchResults(true);
        myHasActiveSimulation = false;
    }
    IsSimulatingPhysics = false;
    
    myListener->Tick();
    
    {
        ZoneScopedN("Dequeue PhysicsCommands");
        for(const Delegate<void(physx::PxPhysics* inPhysics, physx::PxScene* inScene)>& physicsCommand : myPhysicsCommands)
        {
            physicsCommand.Invoke(myPhysics, myScene);
        }
        myPhysicsCommands.Clear();
    }
    
    GetWorld()->GetComponentSystem().TickPhysics();

    IsSimulatingPhysics = true;
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

physx::PxControllerManager* PhysicsSystem::GetControllerManager() const
{
    return myControllerManager;
}

PhysicsListener* PhysicsSystem::GetPhysicsListener() const
{
    return myListener;
}


