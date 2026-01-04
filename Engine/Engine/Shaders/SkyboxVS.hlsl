#include "Shared/MeshStructs.hpp"

[[vk::binding(0, 0)]] ConstantBuffer<CameraBuffer> inCameraBuffer : register(b0);

struct VSInput 
{
    [[vk::location(0)]] float3 position    : POSITION;
    [[vk::location(1)]] int    color       : COLOR;
    [[vk::location(2)]] float3 normal      : NORMAL;
    [[vk::location(3)]] float3 tangent     : TANGENT;
    [[vk::location(4)]] float3 binormal    : BINORMAL;
    [[vk::location(5)]] float2 texCoords0  : TEXCOORD0;
    [[vk::location(6)]] float2 texCoords1  : TEXCOORD1;
};

struct VSOutput 
{
    float4 sv_position : SV_Position;
    [[vk::location(0)]] float2 texCoord     : TEXCOORD0;
    [[vk::location(1)]] float3 outModelFragPos : POSITION1;
};

VSOutput VSMain(VSInput input) 
{
    VSOutput output;

    float4 pos = float4(input.position, 1.0f);
    output.sv_position = mul(inCameraBuffer.myProjection, pos);
    
    float3x3 toViewRotation = (float3x3)inCameraBuffer.myToView;
    output.outModelFragPos = mul(transpose(toViewRotation), input.position);
    
    output.texCoord = input.texCoords0;

    return output;
}