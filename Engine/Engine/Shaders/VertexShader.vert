#version 450

layout(location = 0) in vec4 inPosition;
layout(location = 1) in vec4 inColor;
layout(location = 2) in vec4 inNormal;
layout(location = 3) in vec4 inTangent;
layout(location = 4) in vec4 inBinormals;
layout(location = 5) in vec2 inTexCoords[4];

layout(location = 0) out vec3 outNormal;
layout(location = 1) out vec2 outTexCoord;

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
	vec4 objectPos = inPosition;
	//objectPos.xy *= 10;
	vec4 worldPos = objectPos * myToWorld;
	vec4 viewPos = worldPos * myToView;
	vec4 projPos = viewPos * myProjection;
	gl_Position = projPos;
	outNormal = inNormal.xyz;
	outTexCoord = inTexCoords[0];
}