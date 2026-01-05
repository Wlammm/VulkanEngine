#if __cplusplus
#pragma once
using float4 = glm::vec4;
using float4x4 = glm::mat4;
using float3 = glm::vec3;

#define ALIGNAS(x) alignas(x)
#define DEFAULT_TO(x) = x
#define CPP(x) x
#else
#define CPP(x)
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

enum EDrawFlags CPP(: int)
{
    DrawFlag_NoDepthTest = 1 << 0,
    //DrawFlag_CastShadow = 1 << 1,
    //DrawFlag_AlphaTest  = 1 << 2,
    //DrawFlag_Transparent = 1 << 3,
};

static bool HasFlags(int inValue, int inFlag)
{
    return (inValue & inFlag) != 0;
}

enum EShadingBin CPP(: int)
{
    ShadingBin_Default = 0,
    ShadingBin_NoDepth = 1,
    ShadingBin_Count
};

static bool ShouldDrawForShadingBin(EDrawFlags inDrawFlags, EShadingBin inShadingBin)
{
    bool bNoDepth = HasFlags(inDrawFlags, DrawFlag_NoDepthTest);
    return (bNoDepth && inShadingBin == ShadingBin_NoDepth) ||
           (!bNoDepth && inShadingBin == ShadingBin_Default);
}

// ----------- PushConstants -----------
struct ALIGNAS(16) ShadingBinHeader
{
    ALIGNAS(4) uint myElementsPerBin;
    ALIGNAS(4) EShadingBin myShadingBin;
};

// ----------- ConstantBuffers -----------
struct CameraBuffer
{
    float4x4 myToView;
    float4x4 myProjection;
    float3 myCameraPosition;
};


struct ALIGNAS(16) SceneHeader
{
    ALIGNAS(4) uint myNumMeshInstances DEFAULT_TO(0);
    ALIGNAS(4) uint myNumPointLights DEFAULT_TO(0);
};

struct DirectionalLightBuffer
{
    float4 myColor;
    float3 myDirection;
    float  padding;

    float4x4 myLightView;
    float4x4 myLightProjection;
};

// ----------- StorageBuffers -----------
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

struct ALIGNAS(16) MeshInstanceData
{
    ALIGNAS(16) float4x4 myToWorld;
    ALIGNAS(4) uint myMeshIndex DEFAULT_TO((uint)-1);
    ALIGNAS(4) uint myAlbedoIndex DEFAULT_TO((uint)-1);
    ALIGNAS(4) uint myNormalIndex DEFAULT_TO((uint)-1);
    ALIGNAS(4) uint myMaterialIndex DEFAULT_TO((uint)-1);
    ALIGNAS(4) uint myDrawFlags DEFAULT_TO(0);
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