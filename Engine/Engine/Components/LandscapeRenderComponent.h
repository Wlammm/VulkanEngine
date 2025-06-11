#pragma once
#include "Engine/ComponentSystem/Component.h"
#include "Engine/Math/Heightfield.h"
#include "Engine/Vulkan/GPUSceneSystem.h"

class Mesh;
class VertexBufferHandle;
class IndexBufferHandle;

class LandscapeRenderComponent : public Component
{
    COMP(LandscapeRenderComponent);
public:
    LandscapeRenderComponent();
    ~LandscapeRenderComponent();

    void OnCreate() override;
    
    void Tick() override;

    void OnRenderStateDirty() override;

    const Heightfield& GetHeightfield() const;
    
    // TODO: Should this really be here?
    static constexpr int chunkSize = 64;
    static constexpr float chunkScale = 20000.0f;
    
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
