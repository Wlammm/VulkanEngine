#version 450

layout(location = 0) out vec4 outColor;

layout(location = 0) in vec3 inNormal;
layout(location = 1) in vec2 inTexCoord;

layout(set=2, binding = 0) uniform sampler2D albedo;
layout(set=2, binding = 1) uniform sampler2D normal;
layout(set=2, binding = 2) uniform sampler2D material;

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
    vec3 dirLightDirection = {-1, -1, 0};

    vec3 normal = normalize(inNormal);

    float diffuseFactor = max(dot(normal, -dirLightDirection), 0.2);
	
    //vec4 pointLightColor = vec4(0, 0, 0, 0);
    //for(int i = 0; i < pointLights.length(); ++i)
    //{
    //    pointLightColor += pointLights[i].myColor;
    //}

	outColor = texture(albedo, inTexCoord) * diffuseFactor;
    //outColor = vec4(vec3(1.6, 1.6, 0.6) * diffuseFactor, 1.0);
    //outColor = pointLightColor;
}