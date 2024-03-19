#include "Common.glsl"

vec4 LinearToGamma(vec4 inLinear)
{
    return pow(inLinear, vec4(1.0/2.2));
}

vec4 GammaToLinear(vec4 inGamma)
{
    return pow(inGamma, vec4(2.2));
}

float GetLightFactorFromLightDir(vec3 inFragPos, vec3 inNormal, vec3 inCameraPos, vec4 inLightColor, vec3 inLightDir)
{
    vec3 normal = normalize(inNormal);
    vec3 lightDir = normalize(inLightDir);
    float diffuseFactor = max(dot(inNormal, lightDir), 0.0);
    //return diffuseFactor;
    // Specular
    float shininess = 32;
    vec3 viewDir    = normalize(inCameraPos - inFragPos);
    vec3 halfwayDir = normalize(inLightDir + viewDir);
    float specularFactor = pow(max(dot(normal, halfwayDir), 0.0), shininess);

    return diffuseFactor + specularFactor * when_neq(diffuseFactor, 0.0);
}

vec4 CalculatePointLightColor(vec3 inFragPos, vec3 inNormal, vec3 inCameraPos, vec3 inLightPosition, vec4 inLightColor, float inLightRange)
{
    vec3 lightDir = normalize(inLightPosition - inFragPos);
   
    float distance = length(inLightPosition - inFragPos);
    float linearAttenuation = saturate(1 - (distance / inLightRange));
    float physicalAttenuation = saturate(1.0f / (distance * distance));
    float attenuation = linearAttenuation * physicalAttenuation;

    return inLightColor * GetLightFactorFromLightDir(inFragPos, inNormal, inCameraPos, inLightColor, lightDir) * linearAttenuation;
}

vec4 CalculateAmbientLightColor()
{
    vec4 ambientLightColor = vec4(1, 1, 1, 1);
    float ambientStrength = 0.105;
    vec4 ambientColor = ambientStrength * ambientLightColor;
    return ambientColor;
}

vec4 CalculateDirectionalLightColor(vec3 inFragPos, vec3 inNormal, vec3 inCameraPos, vec3 inLightDirection, vec4 inLightColor, mat4 inLightView, mat4 inLightProjection, sampler2D inDirectionalLightShadowMap)
{
    vec3 lightDir = normalize(inLightDirection);
    
    vec4 lightValue = inLightColor * GetLightFactorFromLightDir(inFragPos, inNormal, inCameraPos, inLightColor, lightDir);

    vec4 fragPosInLightSpace = inLightProjection * inLightView * vec4(inFragPos, 1.0);
    float viewDepth = fragPosInLightSpace.z / fragPosInLightSpace.w;

    vec2 fragPosOnShadowMap = fragPosInLightSpace.xy / fragPosInLightSpace.w / vec2(2.0, 2.0) + vec2(0.5, 0.5);
    //vec2 fragPosOnShadowMap = fragPosInLightSpace.xy / fragPosInLightSpace.w;
    float sampledDepth = texture(inDirectionalLightShadowMap, fragPosOnShadowMap).r;

    float shadowBias = 0.0001;

    if(fragPosOnShadowMap.x > 0.0 && fragPosOnShadowMap.x < 1.0
     && fragPosOnShadowMap.y > 0.0 && fragPosOnShadowMap.y < 1.0)
     {
        return vec4(1, 0, 0, 1);
     }

    // 0 is near, 1 is far
    if(sampledDepth < viewDepth - shadowBias)
    {
        lightValue *= 0;
        //return vec4(0, 0, 0, 0);
    }
    return vec4(1, 1, 1, 1);

    return lightValue;
}