#pragma once
#include "ComponentSystem/Component.h"
#include "Vulkan/GPUSceneSystem.h"

class Mesh;
class Material;
class Model;

class StaticMeshComponent : public Component
{
public:
    void Start() override;
    ~StaticMeshComponent();

    void SetModel(Model* inModel);
    Model* GetModel() const;

    void SetMaterial(Material* inMaterial, const uint inIndex);
    Material* GetMaterial(const uint inIndex) const;

    void SetMaterialForMesh(Material* inMaterial, Mesh* inMesh);
    Material* GetMaterialForMesh(Mesh* inMesh) const;

    void OnRenderStateDirty() override;
    
private:
    void RemoveFromGPUScene();
    
private:
    List<Material*> myMaterials{};
    Model* myModel = nullptr;

    List<MeshInstanceIndex> myMeshInstances{};
};
