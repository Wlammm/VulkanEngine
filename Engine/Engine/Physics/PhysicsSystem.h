#pragma once
#include "Delegates/Delegate.hpp"
#include "System/WorldSystem.h"

namespace physx
{
    class PxOmniPvd;
    class PxErrorCallback;
    class PxTolerancesScale;
    class PxMaterial;
    class PxScene;
    class PxPvdTransport;
    class PxPhysics;
    class PxPvd;
    class PxFoundation;
    class PxDefaultAllocator;
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
    
    physx::PxMaterial* GetDefaultMaterial() const;
    physx::PxScene* GetScene() const;
    
private:
    List<Delegate<void(physx::PxPhysics* inPhysics, physx::PxScene* inScene)>> myPhysicsCommands{};
    
    physx::PxErrorCallback* myDefaultErrorCallback = nullptr;
    physx::PxDefaultAllocator* myDefaultAllocator = nullptr;
    physx::PxTolerancesScale* myToleranceScale = nullptr;
    physx::PxFoundation* myFoundation = nullptr;
    physx::PxMaterial* myDefaultMaterial = nullptr;

    // It is not allowed to add, remove, or modify objects while the simulation is running. Use QueuePhysicsCommand if you want to make changes to these objects.
    physx::PxPhysics* myPhysics = nullptr;
    physx::PxScene* myScene = nullptr;

    bool myWithOmniPvd = false;
    physx::PxOmniPvd* myOmniPvd = nullptr;
    
    bool myHasActiveSimulation = false;
};
