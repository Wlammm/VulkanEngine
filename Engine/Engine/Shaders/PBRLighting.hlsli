#include "Common.hlsli"

float DistributionGGX(float3 N, float3 H, float roughness)
{
    float a      = roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return num / denom;
}

float GeometrySmith(float3 N, float3 V, float3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

float3 fresnelSchlick(float cosTheta, float3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

float3 GetNormalFromMap(float3 inNormalFromNormalTexture, float3 inVertexNormal, float3 inFragWorldPos, float2 inTexCoord)
{
    float3 tangentNormal = inNormalFromNormalTexture * 2.0 - 1.0;

    float3 Q1  = ddx(inFragWorldPos);
    float3 Q2  = ddy(inFragWorldPos);
    float2 st1 = ddx(inTexCoord);
    float2 st2 = ddy(inTexCoord);

    float3 N   = normalize(inVertexNormal);
    float3 T  = normalize(Q1*st2.y - Q2*st1.y);
    float3 B  = -normalize(cross(N, T));
    float3x3 TBN = float3x3(T, B, N);

    return normalize(mul(TBN, tangentNormal));
}

float3 CalculatePointLight(float3 inLightPosition, float3 inLightColor, float inLightRange, float3 inNormal, float3 inCamPos, float3 inFragWorldPos, float3 inAlbedo, float inMetallic, float inRoughness)
{
    float3 lightDirection = inLightPosition - inFragWorldPos;
    
    float3 N = normalize(inNormal);
    float3 V = normalize(inCamPos - inFragWorldPos);

    float3 F0 = float3(0.0f, 0.0f, 0.0f);
    F0 = lerp(F0, inAlbedo, inMetallic);

    // calculate per-light radiance
    float3 L = normalize(lightDirection);
    float3 H = normalize(V + L);
    float distance    = length(lightDirection);
    float physicalAttenuation = 1.0 / (distance * distance);

    float linearAttenuation = distance / inLightRange;
    linearAttenuation = 1.0f - linearAttenuation;
    linearAttenuation = saturate(linearAttenuation);

    float3 radiance     = inLightColor * linearAttenuation * physicalAttenuation;

    // cook-torrance brdf
    float NDF = DistributionGGX(N, H, inRoughness);
    float G   = GeometrySmith(N, V, L, inRoughness);
    float3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);

    float3 kS = F;
    float3 kD = float3(1.0f, 1.0f, 1.0f) - kS;
    kD *= 1.0 - inMetallic;

    float3 numerator    = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
    float3 specular     = numerator / denominator;

    // add to outgoing radiance Lo
    float NdotL = max(dot(N, L), 0.0);

    return (kD * inAlbedo / PI + specular) * radiance * NdotL;
}

float3 CalculateDirectionalLight(float3 inLightDirection, float3 inLightColor, float3 inNormal, float3 inCamPos, float3 inFragWorldPos, float3 inAlbedo, float inMetallic, float inRoughness, RaytracingAccelerationStructure inTLAS)
{
    float3 N = normalize(inNormal);
    float3 V = normalize(inCamPos - inFragWorldPos);

    float3 F0 = float3(0.0f, 0.0f, 0.0f);
    F0 = lerp(F0, inAlbedo, inMetallic);

    if (length(inLightDirection) == 0)
        inLightDirection = float3(1, 0, 0);
    
    // calculate per-light radiance
    float3 L = normalize(inLightDirection);
    float3 H = normalize(V + L);

    // cook-torrance brdf
    float NDF = DistributionGGX(N, H, inRoughness);
    float G   = GeometrySmith(N, V, L, inRoughness);
    float3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);

    float3 kS = F;
    float3 kD = float3(1.0f, 1.0f, 1.0f) - kS;
    kD *= 1.0 - inMetallic;

    float3 numerator    = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
    float3 specular     = numerator / denominator;

    // add to outgoing radiance Lo
    float NdotL = max(dot(N, L), 0.0);

    // Shadow ray: offset origin along normal to avoid self-intersection
    RayDesc shadowRay;
    shadowRay.Origin    = inFragWorldPos + N * 0.01;
    shadowRay.Direction = normalize(-inLightDirection);
    shadowRay.TMin      = 0.001;
    shadowRay.TMax      = 10000.0;
    
    RayQuery<RAY_FLAG_ACCEPT_FIRST_HIT_AND_END_SEARCH | RAY_FLAG_SKIP_CLOSEST_HIT_SHADER | RAY_FLAG_FORCE_OPAQUE> q;
    q.TraceRayInline(inTLAS, 0, 0xFF, shadowRay);
    q.Proceed();
    float shadow = (q.CommittedStatus() == COMMITTED_TRIANGLE_HIT) ? 0.0 : 1.0;

    return inLightColor * (kD * inAlbedo / PI + specular) * NdotL * shadow;
}