#include "Common.glsl"

vec4 CalculatePointLightColor(vec3 inFragPos, vec3 inNormal, vec3 inCameraPos, vec3 inLightPosition, vec4 inLightColor, float inLightRange)
{
    vec3 lightDir = normalize(inLightPosition - inFragPos);
    float diff = max(dot(inNormal, lightDir), 0.0);
    vec4 diffuse = diff * inLightColor;
    
    // Specular
    float shininess = 16;
    vec3 viewDir    = normalize(inCameraPos - inFragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(inNormal, halfwayDir), 0.0), shininess);
    vec4 specular = inLightColor * spec;

    float distance = length(inLightPosition - inFragPos);
    float linearAttenuation = saturate(1 - (distance / inLightRange));
    float physicalAttenuation = saturate(1.0f / (distance * distance));
    float attenuation = linearAttenuation * physicalAttenuation;

    return (diff * inLightColor + specular) * linearAttenuation;
}

vec4 CalculateAmbientLightColor()
{
    vec4 ambientLightColor = vec4(1, 1, 1, 1);
    float ambientStrength = 0.2;
    vec4 ambientColor = ambientStrength * ambientLightColor;
    return ambientColor;
}

vec4 CalculateDirectionalLightColor(vec3 inFragPos, vec3 inNormal, vec3 inCameraPos, vec3 inLightDirection, vec4 inLightColor)
{
    vec3 lightDir = normalize(-inLightDirection);

    float diff = max(dot(inNormal, lightDir), 0.0);
    vec4 diffuse = diff * inLightColor;

    // Specular
    float shininess = 16;
    vec3 viewDir    = normalize(inCameraPos - inFragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(inNormal, halfwayDir), 0.0), shininess);
    vec4 specular = inLightColor * spec;
    
    return diff * inLightColor + specular;
}