// Procedural grass-blade vertex shader (Ghost of Tsushima style). No vertex/index
// streams: each blade is one instance and every vertex is reconstructed from
// SV_VertexID by evaluating a quadratic Bezier curve, tapering the width to the tip.
// The draw is issued once per LOD bin; the push constant carries the segment count and
// the base offset of this bin's region in the shared blade buffer.
#include "Shared/MeshStructs.hpp"
#include "Shared/FoliageStructs.hpp"

struct VSOutput
{
    float4 outPosition : SV_Position;
    [[vk::location(0)]] float3 outNormal : NORMAL;
    [[vk::location(1)]] float2 outTexCoord : TEXCOORD;
    [[vk::location(2)]] nointerpolation uint outTintPacked : TINT;
    [[vk::location(3)]] float outHeightT : HEIGHTT; // 0 at base, 1 at tip
};

[[vk::binding(0, 0)]] ConstantBuffer<CameraBuffer> inCameraBuffer : register(b0);
[[vk::binding(1, 0)]] StructuredBuffer<FoliageBladeData> inFoliageBladeData;

struct BladeDrawConstants
{
    uint mySegments;       // segments in this LOD's blades
    uint myInstanceOffset; // base index of this LOD's region in the blade buffer
};
[[vk::push_constant]] BladeDrawConstants pushConstants;

float3 QuadraticBezier(float3 inP0, float3 inP1, float3 inP2, float inT)
{
    float u = 1.0 - inT;
    return u * u * inP0 + 2.0 * u * inT * inP1 + inT * inT * inP2;
}

float3 QuadraticBezierTangent(float3 inP0, float3 inP1, float3 inP2, float inT)
{
    return 2.0 * (1.0 - inT) * (inP1 - inP0) + 2.0 * inT * (inP2 - inP1);
}

VSOutput VSMain(uint inVertexID : SV_VertexID, uint inInstanceID : SV_InstanceID)
{
    VSOutput output;

    FoliageBladeData blade = inFoliageBladeData[pushConstants.myInstanceOffset + inInstanceID];

    uint segments = pushConstants.mySegments;
    uint tipVertexID = segments * 2u;

    // A blade is a triangle strip of (2*segments + 1) vertices: paired left/right
    // sides walking up the curve, plus a single tip vertex.
    uint seg = inVertexID >> 1u;
    float side = (inVertexID & 1u) ? 1.0 : -1.0;
    if (inVertexID >= tipVertexID)
    {
        seg = segments;
        side = 0.0;
    }
    float t = (float)seg / (float)segments;

    // Blade frame: 'forward' is the facing/bend direction, 'right' is the width axis.
    float s = sin(blade.myYaw);
    float c = cos(blade.myYaw);
    float3 forward = float3(c, 0.0, s);
    float3 up = float3(0.0, 1.0, 0.0);
    float3 rightAxis = normalize(cross(up, forward));

    // Quadratic Bezier: base -> mid control -> tip (curving forward as it rises).
    float3 p0 = blade.myRootPosition;
    float3 p2 = p0 + up * blade.myHeight + forward * blade.myBend;
    float3 p1 = p0 + up * (blade.myHeight * 0.5) + forward * (blade.myBend * 0.25);

    float3 center = QuadraticBezier(p0, p1, p2, t);
    float halfWidth = blade.myWidth * (1.0 - t) * 0.5;
    float3 worldPos = center + rightAxis * side * halfWidth;

    // Surface normal: perpendicular to both the curve tangent and the width axis.
    float3 tangent = normalize(QuadraticBezierTangent(p0, p1, p2, t));
    float3 normal = normalize(cross(tangent, rightAxis));

    output.outPosition = mul(inCameraBuffer.myProjection, mul(inCameraBuffer.myToView, float4(worldPos, 1.0)));
    output.outNormal = normal;
    output.outTexCoord = float2(side * 0.5 + 0.5, t);
    output.outTintPacked = blade.myTintPacked;
    output.outHeightT = t;

    return output;
}
