#pragma once
#include <geometry/PxHeightFieldSample.h>

#include "Engine/Delegates/Delegate.hpp"
#include "Engine/System/WorldSystem.h"

class PhysicsListener;

namespace physx
{
    class PxControllerManager;
    class PxErrorCallback;
    class PxTolerancesScale;
    class PxMaterial;
    class PxScene;
    class PxPvdTransport;
    class PxPhysics;
    class PxPvd;
    class PxFoundation;
    class PxDefaultAllocator;
    PxHeightFieldSample&& PxHeightfieldSample();
}

class PhysicsSystem : public WorldSystem
{
public:
    inline static bool IsSimulatingPhysics = false;

    static void CreateStaticObjects();
    static void DestoryStaticObjects();
    
    PhysicsSystem(World* inWorld);
    ~PhysicsSystem();

    PhysicsSystem(const PhysicsSystem& inOther) = delete;
    PhysicsSystem& operator=(const PhysicsSystem& inOther) = delete;
    PhysicsSystem(PhysicsSystem&& inOther) = delete;
    
    void Tick();

    void QueuePhysicsCommand(Delegate<void(physx::PxPhysics* inPhysics, physx::PxScene* inScene)> inDelegate);

    physx::PxTolerancesScale* GetToleranceScale() const;
    
    physx::PxMaterial* GetDefaultMaterial() const;
    physx::PxScene* GetScene() const;

    physx::PxControllerManager* GetControllerManager() const;

    PhysicsListener* GetPhysicsListener() const;
    
private:
    List<Delegate<void(physx::PxPhysics* inPhysics, physx::PxScene* inScene)>> myPhysicsCommands{};
    
    inline static physx::PxErrorCallback* myDefaultErrorCallback = nullptr;
    inline static physx::PxDefaultAllocator* myDefaultAllocator = nullptr;
    physx::PxTolerancesScale* myToleranceScale = nullptr;
    inline static physx::PxFoundation* myFoundation = nullptr;
    inline static physx::PxPvd* myPvd = nullptr;
    inline static physx::PxPvdTransport* myPvdTransport = nullptr;
    physx::PxMaterial* myDefaultMaterial = nullptr;
    PhysicsListener* myListener;

    // It is not allowed to add, remove, or modify objects while the simulation is running. Use QueuePhysicsCommand if you want to make changes to these objects.
    physx::PxPhysics* myPhysics = nullptr;
    physx::PxScene* myScene = nullptr;
    
    physx::PxControllerManager* myControllerManager = nullptr;

    bool myHasActiveSimulation = false;
};
