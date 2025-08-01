#pragma once
#include "Engine/Assets/Material.h"
#include "Engine/ComponentSystem/Component.h"
#include "Engine/Vulkan/GPUSceneSystem.h"

class Mesh;
class Model;

// Use GENERATED_MATERIAL_PREFIX + albedoPath.string() + normalPath.string() + materialPath.string() for asset registry insertion.
constexpr const char* GENERATED_MATERIAL_PREFIX = "GENERATED_STATICMESHMATERIAL_";

class StaticMeshComponent : public Component
{
public:
    StaticMeshComponent();
    StaticMeshComponent(const std::filesystem::path& inPath);
    void OnCreate() override;
    ~StaticMeshComponent();

    void SetModel(Model* inModel);
    Model* GetModel() const;

    void SetMaterialAsync(const std::filesystem::path& inMaterialPath, const uint inIndex);
    void SetMaterial(Material* inMaterial, const uint inIndex);
    const Material* GetMaterial(const uint inIndex) const;

    void SetMaterialForMesh(Material* inMaterial, Mesh* inMesh);
    const Material* GetMaterialForMesh(Mesh* inMesh) const;

    void OnRenderStateDirty() override;

    void SetDepthWriteEnabled(const bool inEnabled);
    
private:
    void RemoveFromGPUScene();
    
private:
    std::filesystem::path myPath = "";

    META(SerializeField)
    List<Material*> myMaterials{};

    META(SerializeField)
    Model* myModel = nullptr;

    List<MeshInstanceIndex> myMeshInstances{};

    // TODO: This should probably be moved to the material later on. Currently we dont support dynamic material updates in runtime.
    bool myShouldWriteDepth = true;
};
