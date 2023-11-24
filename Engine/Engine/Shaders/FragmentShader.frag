#version 450

layout(location = 0) out vec4 outColor;

layout(location = 0) in vec3 inNormal;
layout(location = 1) in vec2 inTexCoord;

layout(binding = 2) uniform sampler2D texSampler;

void main()
{
    vec3 dirLightDirection = {-1, -1, 0};

    vec3 normal = normalize(inNormal);

    float diffuseFactor = max(dot(normal, -dirLightDirection), 0.2);
	
	outColor= texture(texSampler, inTexCoord) * diffuseFactor;
    outColor = vec4(vec3(0.6, 0.6, 0.6) * diffuseFactor, 1.0);
}