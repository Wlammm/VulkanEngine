#pragma once
#include "Assets/Material.h"
#include "ComponentSystem/Component.h"
#include "Vulkan/GPUSceneSystem.h"

class Mesh;
class Model;

// Use GENERATED_MATERIAL_PREFIX + albedoPath.string() + normalPath.string() + materialPath.string() for asset registry insertion.
constexpr const char* GENERATED_MATERIAL_PREFIX = "GENERATED_STATICMESHMATERIAL_";

class StaticMeshComponent : public Component
{
public:
    void OnCreate() override;
    ~StaticMeshComponent();

    void SetModel(Model* inModel);
    Model* GetModel() const;

    void SetMaterial(Material* inMaterial, const uint inIndex);
    const Material* GetMaterial(const uint inIndex) const;

    void SetMaterialForMesh(Material* inMaterial, Mesh* inMesh);
    const Material* GetMaterialForMesh(Mesh* inMesh) const;

    void OnRenderStateDirty() override;
    
private:
    void RemoveFromGPUScene();
    
private:
    List<Material*> myMaterials{};
    Model* myModel = nullptr;

    List<MeshInstanceIndex> myMeshInstances{};
};
