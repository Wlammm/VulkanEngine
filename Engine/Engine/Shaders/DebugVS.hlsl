
struct VSInput
{
    [[vk::location(0)]] float3 inPosition : POSITION;
    [[vk::location(1)]] int inColor : COLOR;
};

struct VSOutput 
{
    float4 sv_position : SV_Position;
    [[vk::location(0)]] int outColor : COLOR;
};

struct FrameData 
{
    float4x4 myToView;
    float4x4 myProjection;
};

[[vk::binding(0, 0)]] ConstantBuffer<FrameData> inFrameData : register(b0);

VSOutput VSMain(VSInput input)
{
    float4 position = float4(input.inPosition, 1.0f);
    
    VSOutput output;
    output.sv_position = mul(inFrameData.myProjection, mul(inFrameData.myToView, position));
    output.outColor = input.inColor;
    return output;
}
