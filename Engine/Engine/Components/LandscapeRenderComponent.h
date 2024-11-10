#pragma once
#include "ComponentSystem/Component.h"
#include "Vulkan/GPUSceneSystem.h"

class Mesh;
class VertexBuffer;
class IndexBuffer;

class LandscapeRenderComponent : public Component
{
public:
    LandscapeRenderComponent();
    ~LandscapeRenderComponent();

    void Start() override;
    
    void Tick() override;

    void OnRenderStateDirty() override;
    
private:
    void CreateLandscapeMesh();

private:
    VertexBuffer* myVertexBuffer = nullptr;
    IndexBuffer* myIndexBuffer = nullptr;
    Mesh* myMesh = nullptr;
    
    MeshInstanceIndex myMeshInstance = -1;
};
