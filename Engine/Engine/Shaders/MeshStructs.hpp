#if __cplusplus
#pragma once
using vec4 = glm::vec4;
using mat4 = glm::mat4;

#define ALIGNAS(x) alignas(x)
#else
#define ALIGNAS(x) 
#endif

struct ALIGNAS(16) MeshData
{
    vec4 myBoundingSphereModelSpace; // X, Y, Z are center position in model space. W is radius
    uint myVertexOffset;
    uint myIndexOffset;
    uint myIndexCount;
    uint padding;
};

struct ALIGNAS(16) ObjectData
{
    mat4 myToWorld;
    uint myMeshIndex;
};

//struct MeshInstanceData
//{
//    uint myNumObjects;
//    uint myObjectIndices[];
//};