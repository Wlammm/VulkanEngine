#version 460
#include "PBRLighting.glsl"

layout(location = 0) out vec4 outColor;

layout(location = 0) in vec3 inNormal;
layout(location = 1) in vec2 inTexCoord;
layout(location = 2) in vec3 inFragPos;
layout(location = 3) in flat int inDrawID;
layout(location = 4) in vec3 inTangent;
layout(location = 5) in vec3 inBinormals;
layout(location = 6) in vec3 inTangentViewDir;

layout(set = 1, binding = 0) uniform sampler2D textures[];

#include "Buffers/CameraBuffer.hpp"
#include "Buffers/GlobalLightBuffer.hpp"
#include "Buffers/PointLightBuffer.hpp"
#include "Buffers/PerDrawDataBuffer.hpp"

vec3 GetNormalFromNormalTexture(vec3 inSampledNormal, vec3 inWorldPos, vec2 inTexCoord, vec3 inVertexNormal)
{
    vec3 tangentNormal = inSampledNormal * 2.0 - 1.0;

    vec3 Q1  = dFdx(inWorldPos);
    vec3 Q2  = dFdy(inWorldPos);
    vec2 st1 = dFdx(inTexCoord);
    vec2 st2 = dFdy(inTexCoord);

    vec3 N   = normalize(inVertexNormal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

vec3 HDRToLDR(vec3 inColor)
{
    return inColor / (inColor + 1.0f);
}

/*
    Texture packing:
    R: AO
    G: Roughness
    B: Metalness
    A: height (Not implemented yet)
*/
void main()
{
    PerDrawData drawData = inPerDrawData.perDrawData[inDrawID];

    vec2 texCoord = inTexCoord;
    
    vec4 albedoColor = vec4(0.8, 0.2, 1, 1);
    if(drawData.myAlbedoIndex != -1)
    {
        albedoColor = texture(textures[drawData.myAlbedoIndex], texCoord);
        
        if(albedoColor.a == 0)
                discard;
        
        albedoColor = GammaToLinear(albedoColor);
    }

    vec3 normalColor = inNormal;
    if(drawData.myNormalIndex != -1)
    {
        normalColor = GetNormalFromNormalTexture(texture(textures[drawData.myNormalIndex], texCoord).rgb, inFragPos, texCoord, inNormal);
    }
    
    vec4 materialColor = vec4(0, 0, 0, 0);
    if(drawData.myMaterialIndex != -1)
    {
        materialColor = texture(textures[drawData.myMaterialIndex], texCoord);
    }
    
    float roughness = materialColor.g;
    float metalness = materialColor.b;
    
    float ambientLightStrength = 0.1;
    vec3 pointLightColor = vec3(0, 0, 0);

    for(int i = 0; i < inPointLightBuffer.myNumLights; ++i)
    {
        vec3 lightColor = inPointLightBuffer.myLights[i].myColor.rgb * inPointLightBuffer.myLights[i].myIntensity;
        pointLightColor += CalculatePointLight(inPointLightBuffer.myLights[i].myPosition, lightColor, inPointLightBuffer.myLights[i].myRange, normalColor, inCameraBuffer.myCameraPosition, inFragPos, albedoColor.rgb, metalness, roughness);
    }
    
    vec3 directionalLightColor = CalculateDirectionalLight(inGlobalLightBuffer.myDirectionalLightDirection, inGlobalLightBuffer.myDirectionalLightColor.xyz, normalColor, inCameraBuffer.myCameraPosition, inFragPos, albedoColor.rgb, metalness, roughness);
    
    directionalLightColor *= inGlobalLightBuffer.myDirectionalLightColor.a * 10;
    vec3 ambientLight = albedoColor.rgb * ambientLightStrength;
    outColor = vec4(HDRToLDR(LinearToGamma(((ambientLight + directionalLightColor + pointLightColor)))), 1.0);
}
