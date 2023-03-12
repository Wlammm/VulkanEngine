#version 450

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;

layout(location = 0) out vec3 fragColor;

layout(binding = 0) uniform FrameBuffer 
{
	mat4 myToView;
	mat4 myProjection;
};

layout(binding = 1) uniform ObjectBuffer
{
	mat4 myToWorld;
};

void main()
{
	vec4 objectPos = position;
	objectPos.xy *= 10;
	vec4 worldPos = objectPos * myToWorld;
	vec4 viewPos = worldPos * myToView;
	vec4 projPos = viewPos * myProjection;
	gl_Position = projPos;
	fragColor = color.xyz;
}