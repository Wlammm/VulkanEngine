struct PSInput
{
    [[vk::location(0)]] float2 inTexCoord     : TEXCOORD0;
    [[vk::location(1)]] float3 inModelFragPos : TEXCOORD1;
};

[[vk::binding(0, 1)]]
Texture2D textures[];

[[vk::binding(2, 0)]]
SamplerState linearSampler;

// Uniform buffer (set = 0, binding = 0)
cbuffer FrameBuffer : register(b0, space0)
{
    float4x4 myToView;
    float4x4 myProjection;
    float3   myCameraPosition;
    uint     mySkyboxIndex;
};

static const float2 invAtan = float2(0.1591, 0.3183);

float2 SampleSphericalMap(float3 v)
{
    float2 uv = float2(atan2(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;

    uv *= float2(1.0, -1.0);
    return uv;
}

float4 PSMain(PSInput input) : SV_Target
{
    float3 viewDir = normalize(input.inModelFragPos);
    float2 uv = SampleSphericalMap(viewDir);
    return textures[NonUniformResourceIndex(1)].Sample(linearSampler, uv);
}