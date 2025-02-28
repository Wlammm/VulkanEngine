#version 460
#include "MeshStructs.hpp"
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

layout(set=0, binding = 3) uniform sampler2D inDirectionalLightShadowMap;

layout(set = 0, binding = 0) uniform FrameBuffer 
{
	mat4 myToView;
	mat4 myProjection;
	vec3 myCameraPosition;
};

layout(set = 0, binding = 1) readonly buffer PointLightBuffer
{
    uint myNumLights;
    vec3 padding;
    PointLightData myLights[];
} inPointLightBuffer;

layout(set = 0, binding = 2) uniform DirectionalLightBuffer 
{
    vec4 myColor;
    vec3 myDirection;
    float padding;

    mat4 myLightView;
    mat4 myLightProjection;
    
} inDirectionalLightBuffer;

layout(std430, set = 0, binding = 4) readonly buffer PerDrawDataBuffer
{
    PerDrawData perDrawData[];
} inPerDrawData;

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
    
    vec3 albedoColor = vec3(0.8, 0.2, 1);
    if(drawData.myAlbedoIndex != -1)
    {
        albedoColor = texture(textures[drawData.myAlbedoIndex], texCoord).rgb;
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
    
    float ambientLightStrength = 0.005;
    vec3 pointLightColor = vec3(0, 0, 0);

    for(int i = 0; i < inPointLightBuffer.myNumLights; ++i)
    {
        vec3 lightColor = inPointLightBuffer.myLights[i].myColor.rgb * inPointLightBuffer.myLights[i].myIntensity;
        pointLightColor += CalculatePointLight(inPointLightBuffer.myLights[i].myPosition, lightColor, inPointLightBuffer.myLights[i].myRange, normalColor, myCameraPosition, inFragPos, albedoColor, metalness, roughness);
    }
    vec3 lightDir = inDirectionalLightBuffer.myDirection;
    vec3 directionalLightColor = CalculateDirectionalLight(inDirectionalLightBuffer.myDirection, inDirectionalLightBuffer.myColor.xyz, normalColor, myCameraPosition, inFragPos, albedoColor, metalness, roughness);
    
    directionalLightColor *= inDirectionalLightBuffer.myColor.a * 10;
    vec3 ambientLight = albedoColor * ambientLightStrength;
    outColor = vec4(HDRToLDR(LinearToGamma(((ambientLight + directionalLightColor + pointLightColor)))), 1.0);
}