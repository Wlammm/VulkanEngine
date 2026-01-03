#include "Shared/MeshStructs.hpp"
#include "PBRLighting.hlsli"

struct PSInput
{
    [[vk::location(0)]] float3 inNormal : NORMAL;
    [[vk::location(1)]] float2 inTexCoord : TEXCOORD;
    [[vk::location(2)]] float3 inFragPos : FRAGPOS;
    [[vk::location(3)]] int inDrawID : DRAWID;
    [[vk::location(4)]] float3 inTangent : TANGENT;
    [[vk::location(5)]] float3 inBinormals : BINORMALS;
    [[vk::location(6)]] float3 inTangentViewDir : TANGENTVIEWDIR;
};

[[vk::binding(0, 1)]] Texture2D textures[];
[[vk::binding(0, 1)]] SamplerState samplerState;



struct FrameBuffer 
{
	float4x4 myToView;
	float4x4 myProjection;
	float3 myCameraPosition;
    uint myCubemapIndex;
};

[[vk::binding(0, 0)]] ConstantBuffer<FrameBuffer> inFrameData : register(b0);

struct PointLightBuffer
{
    uint myNumLights;
    PointLightData myLights[9999999];
};

// TODO: this could probably be inPointLightBuffer.Count later on instead of myNumLights...
[[vk::binding(1)]]
StructuredBuffer<PointLightBuffer> inPointLightBuffer;

struct DirectionalLightBuffer
{
    float4 myColor;
    float3 myDirection;
    float  padding;

    float4x4 myLightView;
    float4x4 myLightProjection;
};

[[vk::binding(2)]]
ConstantBuffer<DirectionalLightBuffer> inDirectionalLightBuffer : register(b2);

[[vk::binding(4)]]
StructuredBuffer<PerDrawData> inPerDrawData;


float3 GetNormalFromNormalTexture(float3 inSampledNormal, float3 inWorldPos, float2 inTexCoord, float3 inVertexNormal)
{
    float3 tangentNormal = inSampledNormal * 2.0 - 1.0;

    float3 Q1  = ddx(inWorldPos);
    float3 Q2  = ddy(inWorldPos);
    float2 st1 = ddx(inTexCoord);
    float2 st2 = ddy(inTexCoord);

    float3 N   = normalize(inVertexNormal);
    float3 T  = normalize(Q1*st2.y - Q2*st1.y);
    float3 B  = -normalize(cross(N, T));
    float3x3 TBN = float3x3(T, B, N);
    
    return normalize(mul(tangentNormal, TBN));
}

float3 HDRToLDR(float3 inColor)
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
float4 PSMain(PSInput input) : SV_Target
{
    PerDrawData drawData = inPerDrawData[input.inDrawID];

    float2 texCoord = input.inTexCoord;
    
    float4 albedoColor = float4(0.8, 0.2, 1, 1);
    if(drawData.myAlbedoIndex != -1)
    {
        albedoColor = textures[drawData.myAlbedoIndex].Sample(samplerState, texCoord);
        
        if(albedoColor.a == 0)
            discard;
        
        albedoColor = GammaToLinear(albedoColor);
    }

    float3 normalColor = input.inNormal;
    if(drawData.myNormalIndex != -1)
    {
        normalColor = GetNormalFromNormalTexture(textures[drawData.myNormalIndex].Sample(samplerState, texCoord).rgb, input.inFragPos, texCoord, input.inNormal);
    }
    
    float4 materialColor = float4(0, 0, 0, 0);
    if(drawData.myMaterialIndex != -1)
    {
        materialColor = textures[drawData.myMaterialIndex].Sample(samplerState, texCoord);
    }
    
    float roughness = materialColor.g;
    float metalness = materialColor.b;
    
    float ambientLightStrength = 0.1;
    float3 pointLightColor = float3(0, 0, 0);

    for(int i = 0; i < inPointLightBuffer[0].myNumLights; ++i)
    {
        float3 lightColor = inPointLightBuffer[0].myLights[i].myColor.rgb * inPointLightBuffer[0].myLights[i].myIntensity;
        pointLightColor += CalculatePointLight(inPointLightBuffer[0].myLights[i].myPosition, lightColor, inPointLightBuffer[0].myLights[i].myRange, normalColor, inFrameData.myCameraPosition, input.inFragPos, albedoColor.rgb, metalness, roughness);
    }
    float3 lightDir = inDirectionalLightBuffer.myDirection;
    float3 directionalLightColor = CalculateDirectionalLight(inDirectionalLightBuffer.myDirection, inDirectionalLightBuffer.myColor.xyz, normalColor, inFrameData.myCameraPosition, input.inFragPos, albedoColor.rgb, metalness, roughness);
    
    directionalLightColor *= inDirectionalLightBuffer.myColor.a * 10;
    float3 ambientLight = albedoColor.rgb * ambientLightStrength;
    return float4(HDRToLDR(LinearToGamma(((ambientLight + directionalLightColor + pointLightColor)))), 1.0);
}
