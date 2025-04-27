#include "EnginePch.h"
#include "LandscapeColliderComponent.h"

#include <PxPhysics.h>
#include <geometry/PxHeightFieldSample.h>

#include "LandscapeRenderComponent.h"
#include "TransformComponent.h"
#include "Physics/PhysicsSystem.h"
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
        const glm::vec3 scale = GetTransform()->GetScale();

        // Create heightfield collider here.
        List<physx::PxHeightFieldSample> samples{};
        samples.Resize(LandscapeRenderComponent::chunkSize * LandscapeRenderComponent::chunkSize);

        LandscapeRenderComponent* renderComponent = GetComponent<LandscapeRenderComponent>();
        check(renderComponent);
        
        for (int z = 0; z < LandscapeRenderComponent::chunkSize; ++z)
        {
            for (int x = 0; x < LandscapeRenderComponent::chunkSize; ++x)
            {
                float xPos = (static_cast<float>(x) / static_cast<float>(LandscapeRenderComponent::chunkSize - 1) - 0.5f) * LandscapeRenderComponent::chunkScale;
                float zPos = (static_cast<float>(z) / static_cast<float>(LandscapeRenderComponent::chunkSize - 1) - 0.5f) * LandscapeRenderComponent::chunkScale;

                float yPos = renderComponent->GetHeightfield().GetHeight({xPos, zPos});

                physx::PxHeightFieldSample& sample = samples[x * LandscapeRenderComponent::chunkSize + z];
                sample.height = yPos;
            }
        }
        
    });
    
    ColliderComponent::OnCreate();
}
