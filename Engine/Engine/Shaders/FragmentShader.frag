#version 450
#include "CommonLighting.glsl"

layout(location = 0) out vec4 outColor;

layout(location = 0) in vec3 inNormal;
layout(location = 1) in vec2 inTexCoord;
layout(location = 2) in vec3 inFragPos;

layout(set=2, binding = 0) uniform sampler2D albedo;
layout(set=2, binding = 1) uniform sampler2D normal;
layout(set=2, binding = 2) uniform sampler2D material;

layout(set = 0, binding = 0) uniform FrameBuffer 
{
	mat4 myToView;
	mat4 myProjection;
	vec3 myCameraPosition;
};

/*struct PointLightData
{
    vec4 myColor;
    vec3 myPosition;
    float myRange;
};

layout(std140, binding = 3) readonly buffer inPointLightBuffer
{
    PointLightData pointLights[];
};*/

void main()
{
    vec4 albedoColor = texture(albedo, inTexCoord);
    vec3 normal = normalize(inNormal);
	
    vec3 pointLightPosition = vec3(0, 110, 0);
    vec4 pointLightColor = vec4(0, 8, 10, 1);
    float pointLightRange = 500.0;
    
    vec3 dirLightDirection = vec3(0.5, -0.5, 0.5);
    vec4 dirLightColor = vec4(1, 1, 1, 1);

    vec4 pointLightColors = vec4(0, 0, 0, 0);
    pointLightColors += CalculatePointLightColor(inFragPos, normal, myCameraPosition, pointLightPosition, pointLightColor, pointLightRange);
    vec4 directionalLightColors = CalculateDirectionalLightColor(inFragPos, normal, myCameraPosition, dirLightDirection, dirLightColor);

    outColor = (CalculateAmbientLightColor() + directionalLightColors + pointLightColors) * albedoColor;
}