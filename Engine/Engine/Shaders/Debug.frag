#version 450
#include "Common.glsl"

layout(location = 0) in flat int inColor;

layout(location = 0) out vec4 outColor;

void main()
{
    outColor = LinearColorFromColor(inColor);
}