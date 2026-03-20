struct PSInput
{
    [[vk::location(0)]]float2 inTexCoord : TEXCOORD;
};

[[vk::binding(0, 0)]]
Texture2D inTexture;

[[vk::binding(0, 0)]]
SamplerState pointWrapSampler;

float4 PSMain(PSInput input) : SV_Target
{
    return inTexture.Sample(pointWrapSampler, input.inTexCoord);
}