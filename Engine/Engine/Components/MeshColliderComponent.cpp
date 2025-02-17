#include "EnginePch.h"
#include "MeshColliderComponent.h"

#include <PxPhysics.h>
#include <cooking/PxCooking.h>
#include <cooking/PxTriangleMeshDesc.h>
#include <extensions/PxDefaultStreams.h>
#include <geometry/PxTriangleMeshGeometry.h>

#include "Assets/Model.h"
#include "Physics/PhysicsSystem.h"
#include "World/World.h"

void MeshColliderComponent::OnCreate()
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
        List<uint> indices;
        for(const SerializationMeshData& mesh : meshData)
        {
            const uint vertexOffset = vertices.size();
            for(const Vertex& vertex : mesh.myVertices)
            {
                vertices.Add({vertex.myPosition.x, vertex.myPosition.y, vertex.myPosition.z});
            }

            for(const uint index : mesh.myIndices)
            {
                // Offset the indices by the vertices of the other meshes as otherwise the mesh won't be created correctly.
                indices.Add(vertexOffset + index);
            }
        }
        
        physx::PxTriangleMeshDesc desc;
        desc.points.count = vertices.size();
        desc.points.stride = sizeof(glm::vec3);
        desc.points.data = vertices.data();

        desc.triangles.count = indices.size() / 3;
        desc.triangles.stride = sizeof(uint) * 3;
        desc.triangles.data = indices.data();
        
        physx::PxDefaultMemoryOutputStream buf;
        physx::PxCookingParams params = physx::PxCookingParams(*physicsSystem.GetToleranceScale());
        bool result = PxCookTriangleMesh(params, desc, buf);
        check(result && "Failed to cook mesh.");
        physx::PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
        physx::PxTriangleMesh* triangleMesh = inPhysics->createTriangleMesh(input);
        myShape = inPhysics->createShape(physx::PxTriangleMeshGeometry(triangleMesh), *physicsSystem.GetDefaultMaterial());
        triangleMesh->release();
    });
    
    ColliderComponent::OnCreate();
}

void MeshColliderComponent::OnScaleChanged()
{
    check(false);
}

void MeshColliderComponent::SetModel(Model* inModel)
{
    if(myModel == nullptr)
    {
        myModel = inModel;
        OnCreate();
        return;
    }

    OnDestroy();
    myModel = inModel;
    OnCreate();
}
