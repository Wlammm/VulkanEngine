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

    void SetModel(SharedPtr<Model> inModel);
    SharedPtr<Model> GetModel() const;

    void SetMaterialAsync(const std::filesystem::path& inMaterialPath, const uint inIndex);
    
    void SetMaterial(SharedPtr<Material> inMaterial, const uint inIndex);
    void SetMaterial(const std::filesystem::path& inMaterialPath, const uint inIndex);
    
    SharedPtr<Material> GetMaterial(const uint inIndex) const;

    void SetMaterialForMesh(SharedPtr<Material> inMaterial, Mesh* inMesh);
    SharedPtr<Material> GetMaterialForMesh(Mesh* inMesh) const;

    void OnRenderStateDirty() override;

    void SetDepthWriteEnabled(const bool inEnabled);
    
private:
    void RemoveFromGPUScene();
    
private:
    META(SerializeField)
    std::filesystem::path myPath = "";

    // META(SerializeField)
    List<SharedPtr<Material>> myMaterials{};

    META(SerializeField)
    SharedPtr<Model> myModel = nullptr;

    List<MeshInstanceIndex> myMeshInstances{};

    // TODO: This should probably be moved to the material later on. Currently we dont support dynamic material updates in runtime.
    bool myShouldWriteDepth = true;
};
