#ifndef FOLIAGE_STRUCTS_HPP
#define FOLIAGE_STRUCTS_HPP

// Shared between C++ and HLSL. C++ pulls the ALIGNAS/float3/float4x4/DEFAULT_TO
// helpers from MeshStructs.hpp; HLSL shaders must #include "Shared/MeshStructs.hpp"
// before this header so the same macros are already defined.
#if __cplusplus
#pragma once
#include "MeshStructs.hpp"
#endif

/*
 * Compact per-instance foliage record. The full world matrix is reconstructed on
 * the GPU in the cull pass from position/scale/rotation, so this stays small
 * enough to hold millions of instances. See docs/foliage-rendering-plan.md.
 *
 * std430 / HLSL StructuredBuffer stride must match on both sides:
 *   myPosition (vec3, align16) | myScale | myRotationY | 4x uint | pad  => 48 bytes
 */
struct ALIGNAS(16) FoliageInstanceData
{
    ALIGNAS(16) float3 myPosition       DEFAULT_TO(float3(0, 0, 0));
    ALIGNAS(4)  float  myScale          DEFAULT_TO(1.0f);
    ALIGNAS(4)  float  myRotationY      DEFAULT_TO(0.0f); // yaw, radians
    ALIGNAS(4)  uint   myMeshIndex      DEFAULT_TO((uint)-1);
    ALIGNAS(4)  uint   myAlbedoIndex    DEFAULT_TO((uint)-1);
    ALIGNAS(4)  uint   myNormalIndex    DEFAULT_TO((uint)-1);
    ALIGNAS(4)  uint   myMaterialIndex  DEFAULT_TO((uint)-1);
    ALIGNAS(4)  uint   myPadding        DEFAULT_TO(0);
};

// Number of live foliage instances this frame, fed to the cull dispatch.
struct ALIGNAS(16) FoliageSceneHeader
{
    ALIGNAS(4) uint myNumInstances DEFAULT_TO(0);
};

// Per-draw payload produced by the cull pass and consumed by the foliage VS/PS.
struct FoliagePerDrawData
{
    ALIGNAS(16) float4x4 myToWorld;
    ALIGNAS(4) uint myAlbedoIndex;
    ALIGNAS(4) uint myNormalIndex;
    ALIGNAS(4) uint myMaterialIndex;
    ALIGNAS(4) uint myPadding;
};

// Layout-identical to VkDrawIndexedIndirectCommand. Declared with a distinct name
// so shader auto-binding does not collide with the scene's global indirect buffer
// (which is registered under the "DrawIndexedIndirectCommand" alias).
struct FoliageDrawCommand
{
    uint indexCount;
    uint instanceCount;
    uint firstIndex;
    int  vertexOffset;
    uint firstInstance;
};

#endif // FOLIAGE_STRUCTS_HPP
