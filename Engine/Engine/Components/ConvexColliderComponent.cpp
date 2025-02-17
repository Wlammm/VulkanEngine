#include "EnginePch.h"
#include "ConvexColliderComponent.h"
#include "Physics/PhysXInclude.h"
#include "Assets/Model.h"
#include "Physics/PhysicsSystem.h"
#include "World/World.h"

void ConvexColliderComponent::OnCreate()
{
    ZoneScoped;
    
    if(!myModel)
        return;

    PhysicsSystem& physicsSystem = GetWorld()->GetWorldSystem<PhysicsSystem>();
    physicsSystem.QueuePhysicsCommand([this, &physicsSystem](physx::PxPhysics* inPhysics, physx::PxScene* inScene)
    {
        // TODO: Cache cooked physics shapes to improve startup time.
        const List<SerializationMeshData> meshData = Model::GetSerializationDataForModel(myModel);

        List<glm::vec3> vertices;
        for(const SerializationMeshData& mesh : meshData)
        {
            for(const Vertex& vertex : mesh.myVertices)
            {
                vertices.Add({vertex.myPosition.x, vertex.myPosition.y, vertex.myPosition.z});
            }
        }
        
        physx::PxConvexMeshDesc desc;
        desc.points.count = vertices.size();
        desc.points.stride = sizeof(glm::vec3);
        desc.points.data = vertices.data();
        desc.flags = physx::PxConvexFlag::eCOMPUTE_CONVEX;
        
        physx::PxDefaultMemoryOutputStream buf;
        physx::PxCookingParams params = physx::PxCookingParams(*physicsSystem.GetToleranceScale());
        bool result = PxCookConvexMesh(params, desc, buf);
        check(result && "Failed to cook mesh.");
        physx::PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
        physx::PxConvexMesh* convexMesh = inPhysics->createConvexMesh(input);
        myShape = inPhysics->createShape(physx::PxConvexMeshGeometry(convexMesh), *physicsSystem.GetDefaultMaterial());
        convexMesh->release();
    });
    
    ColliderComponent::OnCreate();
}

void ConvexColliderComponent::OnScaleChanged()
{
    
}

void ConvexColliderComponent::SetModel(Model* inModel)
{
    if(myModel == nullptr)
    {
        myModel = inModel;
        OnCreate();
        return;
    }

    // Recreate everything here.
    OnDestroy();
    myModel = inModel;
    OnCreate();
}
