#pragma once

class IndexBuffer;
class VertexBuffer;

class Mesh
{
public:
    VertexBuffer* GetVertexBuffer() const;
    IndexBuffer* GetIndexBuffer() const;
    const glm::vec4& GetSphereBounds() const;

    uint GetHandle() const;

    const std::filesystem::path& GetAlbedoPathTemp() const;

    const std::string& GetAlbedoPath() const;
    const std::string& GetNormalPath() const;
    const std::string& GetMaterialPath() const;
    
private:
    VertexBuffer* myVertexBuffer;
    IndexBuffer* myIndexBuffer;
    glm::vec4 mySphereBounds;

    friend class MeshSystem;
    uint myHandle;
    
    // These are imported from the source file if they exists. They are not necessarily what is being used on the mesh in engine. That is determined by the StaticMeshComponent.
    friend class Model;
    std::string myAlbedoPath;
    std::string myNormalPath;
    std::string myMaterialPath;
};
