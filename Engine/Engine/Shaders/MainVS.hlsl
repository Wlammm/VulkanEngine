#include "Shared/MeshStructs.hpp"

struct VSInput
{
    [[vk::location(0)]] float3 inPosition : POSITION;
    [[vk::location(1)]] int inColor : COLOR;
    [[vk::location(2)]] float3 inNormal : NORMAL;
    [[vk::location(3)]] float3 inTangent : TANGENT;
    [[vk::location(4)]] float3 inBinormals : BINORMAL;
    [[vk::location(5)]] float2 inTexCoords[2] : TEXCOORD;
};

struct VSOutput
{
    float4 outPosition : SV_Position;
    [[vk::location(0)]] float3 outNormal : NORMAL;
    [[vk::location(1)]] float2 outTexCoord : TEXCOORD;
    [[vk::location(2)]] float3 outFragPos : FRAGPOS;
    [[vk::location(3)]] int outDrawID : DRAWID;
    [[vk::location(4)]] float3 outTangent : TANGENT;
    [[vk::location(5)]] float3 outBinormals : BINORMAL;
    [[vk::location(6)]] float3 outTangentViewDir : TANGENTVIEWDIR;
};


struct FrameBuffer
{
    float4x4 myToView;
    float4x4 myProjection;
    float3 myCameraPosition;
};

[[vk::binding(0, 0)]] ConstantBuffer<FrameBuffer> inFrameData : register(b0);

[[vk::binding(4, 0)]] StructuredBuffer<PerDrawData> inPerDrawData;

VSOutput VSMain(VSInput input, [[vk::builtin("DrawIndex")]] uint drawID : SV_InstanceID)
{
    VSOutput output;
    
    PerDrawData drawData = inPerDrawData[drawID];
    output.outPosition = mul(inFrameData.myProjection, mul(inFrameData.myToView, mul(drawData.myToWorld, float4(input.inPosition, 1.0))));
	
    float3x3 toWorldRotation = (float3x3)drawData.myToWorld;
    output.outNormal = normalize(mul(toWorldRotation, input.inNormal));
    output.outTangent = normalize(mul(toWorldRotation, input.inTangent));
    output.outBinormals = normalize(mul(toWorldRotation, input.inBinormals));
    output.outFragPos = (float3)mul(drawData.myToWorld, float4(input.inPosition, 1.0));
    output.outTexCoord = input.inTexCoords[0];
    output.outDrawID = drawID;

    float3 T = normalize(output.outTangent);
    float3 N = normalize(output.outNormal);
    float3 B = cross(N, T);
    float3x3 TBN = transpose(float3x3(T, B, N));
    output.outTangentViewDir = mul(TBN, inFrameData.myCameraPosition - output.outFragPos);
    
    return output;
}