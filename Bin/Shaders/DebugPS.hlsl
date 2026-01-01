#include "Common.hlsli"

struct PSInput
{
    [[vk::location(0)]] int inColor : COLOR;
};

float4 PSMain(PSInput input) : SV_Target
{
    return LinearColorFromColor(input.inColor);
}
