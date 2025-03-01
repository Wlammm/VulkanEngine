#version 460
#include "MeshStructs.hpp"
#include "CommonLighting.glsl"

layout(location = 0) out vec4 outColor;

layout(location = 0) in vec2 inTexCoord;
layout(location = 1) in vec3 inModelFragPos;

layout(set = 1, binding = 0) uniform sampler2D textures[];

layout(set = 0, binding = 0) uniform FrameBuffer
{
    mat4 myToView;
    mat4 myProjection;
    vec3 myCameraPosition;
    uint mySkyboxIndex;
};

const vec2 invAtan = vec2(0.1591, 0.3183);
vec2 SampleSphericalMap(vec3 v)
{
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    
    // Should this be here or is this an overshight somewhere else? 
    uv *= vec2(1, -1);
    return uv;
}

void main()
{
    // We're in model space rotated with the view. The direction would be fragPos - 0, 0, 0 which is unnecessary to do.
    vec3 viewDir = normalize(inModelFragPos);
    outColor = texture(textures[mySkyboxIndex], SampleSphericalMap(viewDir));
}