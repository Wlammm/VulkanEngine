#include "EnginePch.h"
#include "MeshColliderComponent.h"

#include <PxPhysics.h>
#include <cooking/PxCooking.h>
#include <cooking/PxTriangleMeshDesc.h>
#include <extensions/PxDefaultStreams.h>
#include <geometry/PxTriangleMeshGeometry.h>

#include "TransformComponent.h"
#include "Engine/Assets/Model.h"
#include "Engine/Physics/PhysicsSystem.h"
#include "Engine/World/World.h"

void MeshColliderComponent::OnCreate()
{
    ZoneScoped;
    
    if(!myModel)
        return;

    const glm::vec3 scale = GetTransform().GetScale();
    
    PhysicsSystem& physicsSystem = GetWorld()->GetWorldSystem<PhysicsSystem>();
    physicsSystem.QueuePhysicsCommand([this, &physicsSystem, scale](physx::PxPhysics* inPhysics, physx::PxScene* inScene)
    {
        // TODO: Cache cooked physics shapes to improve startup time. Same here as convex collider.
        const List<SerializationMeshData>& meshDatas = myModel->GetSerializationMeshDatas();

        List<physx::PxVec3> vertices;
        List<uint> indices;
        std::unordered_map<glm::vec3, int> indexMap;
        int index = 0;
        for(const SerializationMeshData& mesh : meshDatas)
        {
            check(mesh.myIndices.size() % 3 == 0);
            index++;
            for(const Vertex& vertex : mesh.myVertices)
            {
                if(indexMap.contains(vertex.myPosition))
                    continue;
                
                indexMap.insert({ vertex.myPosition, vertices.size() });
                vertices.Add({vertex.myPosition.x, vertex.myPosition.y, vertex.myPosition.z});
            }

            for(const uint index : mesh.myIndices)
            {
                // Offset the indices by the vertices of the other meshes as otherwise the mesh won't be created correctly.
                const glm::vec3& vertexPos = mesh.myVertices[index].myPosition;
                
                indices.Add(indexMap[vertexPos]);
            }
        }
        
        physx::PxTriangleMeshDesc desc;
        desc.points.count = vertices.size();
        desc.points.stride = sizeof(physx::PxVec3);
        desc.points.data = vertices.data();

        desc.triangles.count = indices.size() / 3;
        desc.triangles.stride = sizeof(uint) * 3;
        desc.triangles.data = indices.data();
        std::cout << "Vertices: " << vertices.size() << ", Indices: " << indices.size() << "\n";
        std::cout << "Triangles: " << indices.size() / 3 << "\n";
        
        physx::PxDefaultMemoryOutputStream buf;
        physx::PxCookingParams params = physx::PxCookingParams(*physicsSystem.GetToleranceScale());
        params.meshPreprocessParams.raise(physx::PxMeshPreprocessingFlag::eFORCE_32BIT_INDICES);

        bool res = PxValidateTriangleMesh(params, desc);
        check(res);
        
        bool result = PxCookTriangleMesh(params, desc, buf);
        check(result && "Failed to cook mesh.");
        physx::PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
        physx::PxTriangleMesh* triangleMesh = inPhysics->createTriangleMesh(input);

        physx::PxMeshScale pxScale = physx::PxMeshScale({scale.x, scale.y, scale.z });
            
        myShape = inPhysics->createShape(physx::PxTriangleMeshGeometry(triangleMesh, pxScale), *physicsSystem.GetDefaultMaterial());
        triangleMesh->release();
    });
    
    ColliderComponent::OnCreate();
}

void MeshColliderComponent::OnScaleChanged()
{
    OnDestroy();
    OnCreate();
    
    return;
    const glm::vec3 scale = GetTransform().GetScale();

    // Make sure the box is not of 0 size. TODO: This shouldn't be a crash later but for now it is .
    check(scale.length() > 0);
    
    GetWorld()->GetWorldSystem<PhysicsSystem>().QueuePhysicsCommand([this, scale](physx::PxPhysics *,physx::PxScene *)
    {
        physx::PxTriangleMeshGeometry geometry = static_cast<const physx::PxTriangleMeshGeometry&>(myShape->getGeometry());
        geometry.scale = physx::PxMeshScale({scale.x, scale.y, scale.z });
        myShape->setGeometry(geometry);
    });  
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
