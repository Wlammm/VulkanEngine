#include "SharedHeader.hpp"

#ifndef MESHSTRUCTS_H
#define MESHSTRUCTS_H

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
    ALIGNAS(16) mat4 myToWorld;
    ALIGNAS(4) uint myMeshIndex DEFAULT_TO((uint)-1);
    ALIGNAS(4) uint myAlbedoIndex DEFAULT_TO((uint)-1);
    ALIGNAS(4) uint myNormalIndex DEFAULT_TO((uint)-1);
    ALIGNAS(4) uint myMaterialIndex DEFAULT_TO((uint)-1);
    ALIGNAS(4) int myDepthWriteEnabled DEFAULT_TO(1);
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
    ALIGNAS(4) float myIntensity;
};

#endif