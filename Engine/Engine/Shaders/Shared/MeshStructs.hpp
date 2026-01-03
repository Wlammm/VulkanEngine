#if __cplusplus
#pragma once
using float4 = glm::vec4;
using float4x4 = glm::mat4;
using float3 = glm::vec3;

#define ALIGNAS(x) alignas(x)
#define DEFAULT_TO(x) = x
#else
#define ALIGNAS(x)
#define DEFAULT_TO(x) 
#endif

/*
 * Alignment rules in glsl:
 * - scalar             = 4
 * - vec2               = 8
 * - vec3 & float4 & float4x4 = 16
 * A nested structure must be aligned by the base alignment of its members rounded up to a multiple of 16
 */

struct MeshData
{
    ALIGNAS(16) float4 myBoundingSphereModelSpace; // X, Y, Z are center position in model space. W is radius
    ALIGNAS(4) uint myVertexIndex; // The index into the global sparse vertex data buffer.
    ALIGNAS(4) uint myIndexDataIndex; // The index into the global sparse index data buffer.
    ALIGNAS(4) uint myMaterialIndex; // TODO: Is this even used?
};

struct VertexBufferData
{
    ALIGNAS(4) uint myOffset;
};

struct IndexBufferData
{
    ALIGNAS(4) uint myOffset;
    ALIGNAS(4) uint myCount;
};

struct ALIGNAS(16) SceneHeader
{
    ALIGNAS(4) uint myNumMeshInstances DEFAULT_TO(0);
    ALIGNAS(4) uint myNumPointLights DEFAULT_TO(0);
};

struct ALIGNAS(16) MeshInstanceData
{
    ALIGNAS(16) float4x4 myToWorld;
    ALIGNAS(4) uint myMeshIndex DEFAULT_TO((uint)-1);
    ALIGNAS(4) uint myAlbedoIndex DEFAULT_TO((uint)-1);
    ALIGNAS(4) uint myNormalIndex DEFAULT_TO((uint)-1);
    ALIGNAS(4) uint myMaterialIndex DEFAULT_TO((uint)-1);
    ALIGNAS(4) int myDepthWriteEnabled DEFAULT_TO(1);
};

struct PerDrawData
{
    ALIGNAS(16) float4x4 myToWorld;
    ALIGNAS(4) uint myAlbedoIndex;
    ALIGNAS(4) uint myNormalIndex;
    ALIGNAS(4) uint myMaterialIndex;
    ALIGNAS(4) uint padding;
};

struct PointLightData
{
    ALIGNAS(16) float4 myColor;
    ALIGNAS(16) float3 myPosition;
    ALIGNAS(4) float myRange;
    ALIGNAS(4) float myIntensity;
};