#include "Shared/MeshStructs.hpp"

struct PSInput
{
    [[vk::location(0)]] float3 inNormal : NORMAL;
};

[[vk::binding(2, 0)]] ConstantBuffer<DirectionalLightBuffer> inDirectionalLightBuffer : register(b2);

// Phase 1 placeholder shading: flat grass-green with simple directional + ambient
// lighting. Material/texture sampling is wired up in a later phase.
float4 PSMain(PSInput input) : SV_Target
{
    const float3 baseColor = float3(0.22, 0.55, 0.16);
    const float ambient = 0.25;

    float3 normal = normalize(input.inNormal);
    float3 lightDir = normalize(-inDirectionalLightBuffer.myDirection);
    float ndl = saturate(dot(normal, lightDir));

    float3 lightColor = inDirectionalLightBuffer.myColor.rgb * inDirectionalLightBuffer.myColor.a;
    float3 lit = baseColor * (ambient + ndl * lightColor);

    return float4(lit, 1.0);
}
