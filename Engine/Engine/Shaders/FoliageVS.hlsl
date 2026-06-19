#include "Shared/MeshStructs.hpp"
#include "Shared/FoliageStructs.hpp"

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
    [[vk::location(2)]] int outDrawID : DRAWID;
};

[[vk::binding(0, 0)]] ConstantBuffer<CameraBuffer> inCameraBuffer : register(b0);
[[vk::binding(1, 0)]] StructuredBuffer<FoliagePerDrawData> inFoliagePerDrawData;

VSOutput VSMain(VSInput input, [[vk::builtin("DrawIndex")]] uint inDrawID : SV_InstanceID)
{
    VSOutput output;

    FoliagePerDrawData drawData = inFoliagePerDrawData[inDrawID];

    output.outPosition = mul(inCameraBuffer.myProjection, mul(inCameraBuffer.myToView, mul(drawData.myToWorld, float4(input.inPosition, 1.0))));

    float3x3 toWorldRotation = (float3x3)drawData.myToWorld;
    output.outNormal = normalize(mul(toWorldRotation, input.inNormal));
    output.outTexCoord = input.inTexCoords[0];
    output.outDrawID = inDrawID;

    return output;
}
