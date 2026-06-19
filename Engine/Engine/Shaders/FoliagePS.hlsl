#include "Shared/MeshStructs.hpp"
#include "Shared/FoliageStructs.hpp"
#include "Common.hlsli"

struct PSInput
{
    [[vk::location(0)]] float3 inNormal : NORMAL;
    [[vk::location(1)]] float2 inTexCoord : TEXCOORD;
    [[vk::location(2)]] int inDrawID : DRAWID;
};

[[vk::binding(1, 0)]] StructuredBuffer<FoliagePerDrawData> inFoliagePerDrawData;
[[vk::binding(2, 0)]] ConstantBuffer<DirectionalLightBuffer> inDirectionalLightBuffer : register(b2);

// Phase 2 shading: per-type tint with simple directional + ambient lighting.
// Material/texture sampling slots (myAlbedoIndex/myNormalIndex) are reserved on the
// per-draw data and wired up when foliage types gain real materials.
float4 PSMain(PSInput input) : SV_Target
{
    FoliagePerDrawData drawData = inFoliagePerDrawData[input.inDrawID];

    float3 baseColor = LinearColorFromColor((int)drawData.myTintPacked).rgb;

    float3 normal = normalize(input.inNormal);
    float3 lightDir = normalize(-inDirectionalLightBuffer.myDirection);
    float ndl = saturate(dot(normal, lightDir));

    float3 lightColor = inDirectionalLightBuffer.myColor.rgb * inDirectionalLightBuffer.myColor.a;
    const float ambient = 0.3;
    float3 lit = baseColor * (ambient + ndl * lightColor);

    return float4(lit, 1.0);
}
