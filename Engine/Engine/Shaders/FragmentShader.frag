#version 450
#include "CommonLighting.glsl"

layout(location = 0) out vec4 outColor;

layout(location = 0) in vec3 inNormal;
layout(location = 1) in vec2 inTexCoord;
layout(location = 2) in vec3 inFragPos;

layout(set=2, binding = 0) uniform sampler2D albedo;
layout(set=2, binding = 1) uniform sampler2D normal;
layout(set=2, binding = 2) uniform sampler2D material;
layout(set=0, binding = 3) uniform sampler2D inDirectionalLightShadowMap;

layout(set = 0, binding = 0) uniform FrameBuffer 
{
	mat4 myToView;
	mat4 myProjection;
	vec3 myCameraPosition;
};

struct PointLight
{
    vec4 myColor;
    vec3 myPosition;
    float myRange;
};
layout(std430, set = 0, binding = 1) readonly buffer PointLightBuffer
{
    int myNumLights;
    PointLight myLights[10];
} inPointLightBuffer;

layout(set = 0, binding = 2) uniform DirectionalLightBuffer 
{
    vec4 myColor;
    vec3 myDirection;
    float padding;

    mat4 myLightView;
    mat4 myLightProjection;
    
} inDirectionalLightBuffer;

void main()
{
    outColor = vec4(inPointLightBuffer.myLights[0].myPosition, 1.0);
    vec4 albedoColor = texture(albedo, inTexCoord);
    vec3 normal = normalize(inNormal);

    vec4 pointLightColors = vec4(0, 0, 0, 0);

    /*for(int i = 0; i < inPointLightBuffer.myNumLights; ++i)
    {
        pointLightColors += CalculatePointLightColor(inFragPos, normal, myCameraPosition, inPointLightBuffer.myLights[i].myPosition, inPointLightBuffer.myLights[i].myColor, inPointLightBuffer.myLights[i].myRange);
    }*/

    vec4 directionalLightColors = CalculateDirectionalLightColor(inFragPos, normal, myCameraPosition, inDirectionalLightBuffer.myDirection, inDirectionalLightBuffer.myColor, inDirectionalLightBuffer.myLightView, inDirectionalLightBuffer.myLightProjection, inDirectionalLightShadowMap);

    outColor = LinearToGamma((CalculateAmbientLightColor() + directionalLightColors + pointLightColors) * albedoColor);
}