#include "Shared/MeshStructs.hpp"
#include "Shared/FoliageStructs.hpp"
#include "Common.hlsli"

struct PSInput
{
    [[vk::location(0)]] float3 inNormal : NORMAL;
    [[vk::location(1)]] float2 inTexCoord : TEXCOORD;
    [[vk::location(2)]] nointerpolation uint inTintPacked : TINT;
    [[vk::location(3)]] float inHeightT : HEIGHTT;
};

[[vk::binding(2, 0)]] ConstantBuffer<DirectionalLightBuffer> inDirectionalLightBuffer : register(b2);

// Phase 2 shading: per-type tint with simple two-sided directional + ambient lighting.
// Blades are drawn double-sided (no back-face culling), so the normal is flipped for
// back faces. A cheap vertical AO term darkens the blade toward its base.
float4 PSMain(PSInput input, bool inIsFrontFace : SV_IsFrontFace) : SV_Target
{
    float3 baseColor = LinearColorFromColor((int)input.inTintPacked).rgb;

    float3 normal = normalize(input.inNormal);
    if (!inIsFrontFace)
        normal = -normal;

    float3 lightDir = normalize(-inDirectionalLightBuffer.myDirection);
    float ndl = saturate(dot(normal, lightDir));

    float3 lightColor = inDirectionalLightBuffer.myColor.rgb * inDirectionalLightBuffer.myColor.a;
    const float ambient = 0.3;
    float ao = lerp(0.6, 1.0, input.inHeightT); // darker at the base
    float3 lit = baseColor * ao * (ambient + ndl * lightColor);

    return float4(lit, 1.0);
}
