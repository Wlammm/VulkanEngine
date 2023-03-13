#version 450

layout(location = 0) out vec4 outColor;

layout(location = 0) in vec3 inNormal;

void main()
{
    vec3 dirLightDirection = {-1, 1, 0};

    vec3 normal = normalize(inNormal);

    float diffuseFactor = max(dot(normal, -dirLightDirection), 0.2);

    outColor = vec4(vec3(1, 1, 1) * diffuseFactor, 1.0);
}