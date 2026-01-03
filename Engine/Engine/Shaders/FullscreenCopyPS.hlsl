struct PSInput
{
    [[vk::location(0)]]float2 inTexCoord : TEXCOORD;
};

[[vk::binding(0, 0)]]
Texture2D inTexture;

[[vk::binding(0, 0)]]
SamplerState textureSampler;

float4 PSMain(PSInput input) : SV_Target
{
    return inTexture.Sample(textureSampler, input.inTexCoord);
}