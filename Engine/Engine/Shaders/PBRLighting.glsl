#include "Common.glsl"

float DistributionGGX(vec3 N, vec3 H, float roughness)
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

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

vec3 GetNormalFromMap(vec3 inNormalFromNormalTexture, vec3 inVertexNormal, vec3 inFragWorldPos, vec2 inTexCoord)
{
    vec3 tangentNormal = inNormalFromNormalTexture * 2.0 - 1.0;

    vec3 Q1  = dFdx(inFragWorldPos);
    vec3 Q2  = dFdy(inFragWorldPos);
    vec2 st1 = dFdx(inTexCoord);
    vec2 st2 = dFdy(inTexCoord);

    vec3 N   = normalize(inVertexNormal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

// inLightDirection = lightPositions[i] - inFragWorldPos; (Non normalized!!)
vec3 CalculateDirectionalLight(vec3 inLightDirection, vec3 inLightColor, float inLightRange, vec3 inNormal, vec3 inCamPos, vec3 inFragWorldPos, vec3 inAlbedo, float inMetallic, float inRoughness)
{
    vec3 N = normalize(inNormal);
    vec3 V = normalize(inCamPos - inFragWorldPos);

    vec3 F0 = vec3(0.0);
    F0 = mix(F0, inAlbedo, inMetallic);

    // calculate per-light radiance
    vec3 L = normalize(inLightDirection);
    vec3 H = normalize(V + L);
    float distance    = length(inLightDirection);
    float physicalAttenuation = 1.0 / (distance * distance);

    float linearAttenuation = distance / inLightRange;
    linearAttenuation = 1.0f - linearAttenuation;
    linearAttenuation = saturate(linearAttenuation);

    vec3 radiance     = inLightColor * linearAttenuation * physicalAttenuation;

    // cook-torrance brdf
    float NDF = DistributionGGX(N, H, inRoughness);
    float G   = GeometrySmith(N, V, L, inRoughness);
    vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);

    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - inMetallic;

    vec3 numerator    = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
    vec3 specular     = numerator / denominator;

    // add to outgoing radiance Lo
    float NdotL = max(dot(N, L), 0.0);

    return (kD * inAlbedo / PI + specular) * radiance * NdotL;
}