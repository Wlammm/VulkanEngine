#include "EnginePch.h"
#include "LandscapeColliderComponent.h"

#include <PxPhysics.h>
#include <cooking/PxCooking.h>
#include <extensions/PxRigidActorExt.h>
#include <geometry/PxHeightFieldDesc.h>
#include <geometry/PxHeightFieldSample.h>

#include "LandscapeRenderComponent.h"
#include "TransformComponent.h"
#include "Physics/PhysicsSystem.h"
#include "Utils/Debug.h"
#include "World/World.h"

void LandscapeColliderComponent::OnCreate()
{
    LandscapeRenderComponent* renderComponent = GetComponent<LandscapeRenderComponent>();
    if(!renderComponent)
    {
        GetGameObject()->OnComponentAdded.Bind(&LandscapeColliderComponent::OnComponentAdded, this);
        return;
    }

    CreateCollider();
}

void LandscapeColliderComponent::Tick()
{
    ColliderComponent::Tick();
    Debug::DrawLine(GetTransform()->GetPosition(), GetTransform()->GetPosition() + glm::vec3{0, 10000, 0});
}

void LandscapeColliderComponent::OnScaleChanged()
{
    check(false); // TODO: Implement this
}

void LandscapeColliderComponent::OnComponentAdded(Component* inComponent)
{
    if(inComponent->IsA<LandscapeRenderComponent>())
        CreateCollider();
}

void LandscapeColliderComponent::CreateCollider()
{
    PhysicsSystem& physicsSystem = GetWorld()->GetWorldSystem<PhysicsSystem>();
    
    physicsSystem.QueuePhysicsCommand([this, &physicsSystem](physx::PxPhysics* inPhysics, physx::PxScene* inScene)
    {
        constexpr int chunkSize = LandscapeRenderComponent::chunkSize * 5;
        
        const glm::vec3 scale = GetTransform()->GetScale();
        
        // Create heightfield collider here.
        List<physx::PxHeightFieldSample> samples{};
        samples.Resize(chunkSize * chunkSize);

        LandscapeRenderComponent* renderComponent = GetComponent<LandscapeRenderComponent>();
        check(renderComponent);
        
        for (int z = 0; z < chunkSize; ++z)
        {
            for (int x = 0; x < chunkSize; ++x)
            {
                float xPos = (static_cast<float>(x) / static_cast<float>(chunkSize - 1) - 0.5f) * LandscapeRenderComponent::chunkScale;
                float zPos = (static_cast<float>(z) / static_cast<float>(chunkSize - 1) - 0.5f) * LandscapeRenderComponent::chunkScale;

                float yPos = renderComponent->GetHeightfield().GetHeight({xPos, zPos});

                physx::PxHeightFieldSample& sample = samples[x * chunkSize + z];
                // TODO: This might give inaccuracies. We could add more granularity by using the height scale.
                sample.height = static_cast<int>(yPos * 10.0f);
                
                // We can use the predefined material if we want to add a hole in a specific part of the landscape. 
                // sample.materialIndex0 = physx::PxHeightFieldMaterial::eHOLE;
            }
        }

        physx::PxHeightFieldDesc desc;
        desc.format = physx::PxHeightFieldFormat::eS16_TM;
        desc.nbColumns = chunkSize;
        desc.nbRows = chunkSize;
        desc.samples.data = samples.data();
        desc.samples.stride = sizeof(physx::PxHeightFieldSample);

        physx::PxHeightField* heightField = PxCreateHeightField(desc, inPhysics->getPhysicsInsertionCallback());

        const float chunkScale = LandscapeRenderComponent::chunkScale / chunkSize;
        physx::PxHeightFieldGeometry heightFieldGeometry = physx::PxHeightFieldGeometry(heightField, physx::PxMeshGeometryFlags(), 0.1f, chunkScale, chunkScale);
        myShape = inPhysics->createShape(heightFieldGeometry, *physicsSystem.GetDefaultMaterial(), true);
    });

    SetPhysicsOffset({-LandscapeRenderComponent::chunkScale * 0.5f, 0, -LandscapeRenderComponent::chunkScale * 0.5f});
    ColliderComponent::OnCreate();
}
