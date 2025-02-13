#pragma once
#include "Assets/Material.h"
#include "ComponentSystem/Component.h"
#include "Vulkan/GPUSceneSystem.h"

class Mesh;
class Model;

class StaticMeshComponent : public Component
{
public:
    void OnCreate() override;
    ~StaticMeshComponent();

    bool DoesComponentImplementOnRenderStateDirty() const override { return true; }
    
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
    List<Material> myMaterials{};
    Model* myModel = nullptr;

    List<MeshInstanceIndex> myMeshInstances{};
};
