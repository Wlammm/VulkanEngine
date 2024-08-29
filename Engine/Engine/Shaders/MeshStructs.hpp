#if __cplusplus
#pragma once
using vec4 = glm::vec4;
using mat4 = glm::mat4;
using vec3 = glm::vec3;

#define ALIGNAS(x) alignas(x)
#else
#define ALIGNAS(x) 
#endif

/*
 * Alignment rules in glsl:
 * - scalar             = 4
 * - vec2               = 8
 * - vec3 & vec4 & mat4 = 16
 * A nested structure must be aligned by the base alignment of its members rounded up to a multiple of 16
 */

struct MeshData
{
    ALIGNAS(16) vec4 myBoundingSphereModelSpace; // X, Y, Z are center position in model space. W is radius
    ALIGNAS(4) uint myVertexOffset;
    ALIGNAS(4) uint myIndexOffset;
    ALIGNAS(4) uint myIndexCount;
    ALIGNAS(4) uint myMaterialIndex;
};

struct ALIGNAS(16) ObjectData
{
    ALIGNAS(16) mat4 myToWorld;
    ALIGNAS(4) uint myMeshIndex;
    ALIGNAS(4) uint myAlbedoIndex;
    ALIGNAS(4) uint myNormalIndex;
    ALIGNAS(4) uint myMaterialIndex;
};

struct PerDrawData
{
    ALIGNAS(16) mat4 myToWorld;
    ALIGNAS(4) uint myAlbedoIndex;
    ALIGNAS(4) uint myNormalIndex;
    ALIGNAS(4) uint myMaterialIndex;
    ALIGNAS(4) uint padding;
};

struct PointLightData
{
    ALIGNAS(16) vec4 myColor;
    ALIGNAS(16) vec3 myPosition;
    ALIGNAS(4) float myRange;
};