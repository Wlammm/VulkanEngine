#pragma once
#include "ComponentSystem/Component.h"
#include "Math/Heightfield.h"
#include "Vulkan/GPUSceneSystem.h"

class Mesh;
class VertexBufferHandle;
class IndexBufferHandle;

class LandscapeRenderComponent : public Component
{
public:
    LandscapeRenderComponent();
    ~LandscapeRenderComponent();

    void OnCreate() override;
    
    void Tick() override;

    void OnRenderStateDirty() override;
    
private:
    void CreateLandscapeMesh();

private:
    Heightfield myHeightfield{};
    
    VertexBufferHandle* myVertexBuffer = nullptr;
    IndexBufferHandle* myIndexBuffer = nullptr;
    Mesh* myMesh = nullptr;
    
    MeshInstanceIndex myMeshInstance = -1;
    
    Material* myMaterial = nullptr;
};
