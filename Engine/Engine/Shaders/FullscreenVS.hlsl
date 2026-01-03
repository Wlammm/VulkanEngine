static const float4 pos[3] =
{
    float4(1.0f, 3.0f, 0.0f, 1.0f),
    float4(-3.0f, -1.0f, 0.0f, 1.0f),
    float4(1.0f, -1.0f, 0.0f, 1.0f),
};

static const float2 uv[3] =
{
    float2(2.0, 2.0),
    float2(-0.0, -0.0),
    float2(2.0, -0),
};

struct VSOutput
{
    float4 outPosition : SV_Position;
    [[vk::location(0)]]float2 outTexCoord : TEXCOORD;
};

VSOutput VSMain(uint inVertexID : SV_VertexID)
{
    VSOutput output;
    output.outTexCoord = uv[inVertexID];
    output.outPosition = pos[inVertexID];
    return output;
}