#pragma once
#include <geometry/PxHeightFieldSample.h>

#include "Delegates/Delegate.hpp"
#include "System/WorldSystem.h"

class PhysicsListener;

namespace physx
{
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
    
private:
    List<Delegate<void(physx::PxPhysics* inPhysics, physx::PxScene* inScene)>> myPhysicsCommands{};
    
    physx::PxErrorCallback* myDefaultErrorCallback = nullptr;
    physx::PxDefaultAllocator* myDefaultAllocator = nullptr;
    physx::PxTolerancesScale* myToleranceScale = nullptr;
    physx::PxFoundation* myFoundation = nullptr;
    physx::PxPvd* myPvd = nullptr;
    physx::PxPvdTransport* myPvdTransport = nullptr;
    physx::PxMaterial* myDefaultMaterial = nullptr;
    PhysicsListener* myListener;

    // It is not allowed to add, remove, or modify objects while the simulation is running. Use QueuePhysicsCommand if you want to make changes to these objects.
    physx::PxPhysics* myPhysics = nullptr;
    physx::PxScene* myScene = nullptr;

    bool myHasActiveSimulation = false;
};
