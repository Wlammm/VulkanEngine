#ifndef FOLIAGE_STRUCTS_HPP
#define FOLIAGE_STRUCTS_HPP

// Shared between C++ and HLSL. C++ pulls the ALIGNAS/float3/float4x4/DEFAULT_TO
// helpers from MeshStructs.hpp; HLSL shaders must #include "Shared/MeshStructs.hpp"
// before this header so the same macros are already defined.
#if __cplusplus
#pragma once
#include "MeshStructs.hpp"
#endif

// -------------------- Blade LOD table --------------------
// Grass blades are generated procedurally in the vertex shader (Ghost of Tsushima
// style): the cull pass bins survivors by distance into FOLIAGE_LOD_COUNT levels of
// detail, and each level draws blades with a fixed segment count. A blade with S
// segments is a triangle strip of (2*S + 1) vertices (paired sides + a single tip).
#if __cplusplus
static constexpr uint FOLIAGE_LOD_COUNT = 3;
static constexpr uint FOLIAGE_LOD_SEGMENTS[FOLIAGE_LOD_COUNT] = { 5u, 3u, 1u };
#else
static const uint FOLIAGE_LOD_COUNT = 3;
static const uint FOLIAGE_LOD_SEGMENTS[FOLIAGE_LOD_COUNT] = { 5u, 3u, 1u };
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
    ALIGNAS(4)  uint   myTintPacked     DEFAULT_TO(0xFFFFFFFF); // per-type tint, RGBA8
};

// Number of live foliage instances this frame, fed to the cull dispatch.
struct ALIGNAS(16) FoliageSceneHeader
{
    ALIGNAS(4) uint myNumInstances DEFAULT_TO(0);
    ALIGNAS(4) uint myBinCapacity  DEFAULT_TO(0); // max blades per LOD bin (blade-buffer stride between bins)
};

// Runtime scalability knobs read by the cull pass. All adjustable live with no
// rebuild, since culling/density decisions are made per-frame on the GPU.
struct ALIGNAS(16) FoliageScalabilitySettings
{
    ALIGNAS(4) float myGlobalMaxDistanceScale DEFAULT_TO(1.0f); // multiplies max + fade distances
    ALIGNAS(4) float myGlobalDensityScale     DEFAULT_TO(1.0f); // 0..1 fraction of instances kept
    ALIGNAS(4) float myMaxDrawDistance        DEFAULT_TO(14000.0f);
    ALIGNAS(4) float myFadeStartDistance      DEFAULT_TO(7000.0f); // density thins from here to max
};

// Compact per-blade record produced by the cull pass, consumed by the foliage VS.
// The VS reconstructs the full blade geometry (a quadratic Bezier) from these fields;
// no world matrix is stored. 32 bytes.
struct ALIGNAS(16) FoliageBladeData
{
    ALIGNAS(16) float3 myRootPosition DEFAULT_TO(float3(0, 0, 0)); // world-space blade base
    ALIGNAS(4)  float  myHeight       DEFAULT_TO(1.0f);
    ALIGNAS(4)  float  myWidth        DEFAULT_TO(0.1f);            // width at the base, tapers to the tip
    ALIGNAS(4)  float  myBend         DEFAULT_TO(0.0f);            // forward bend distance of the tip
    ALIGNAS(4)  float  myYaw          DEFAULT_TO(0.0f);            // facing / bend direction, radians
    ALIGNAS(4)  uint   myTintPacked   DEFAULT_TO(0xFFFFFFFF);     // RGBA8
};

// Layout-identical to VkDrawIndirectCommand (non-indexed). Blades are procedural, so
// the draw uses no vertex/index streams. Declared with a distinct name so shader
// auto-binding does not collide with the scene's "DrawIndexedIndirectCommand" alias.
struct FoliageDrawCommand
{
    uint vertexCount;
    uint instanceCount;
    uint firstVertex;
    uint firstInstance;
};

#endif // FOLIAGE_STRUCTS_HPP
