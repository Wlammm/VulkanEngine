#version 450

layout(location = 0) in vec4 inPosition;
layout(location = 1) in vec4 inColor;
layout(location = 2) in vec4 inNormal;
layout(location = 3) in vec4 inTangent;
layout(location = 4) in vec4 inBinormals;
layout(location = 5) in vec2 inTexCoords[4];

layout(location = 0) out vec3 outNormal;
layout(location = 1) out vec2 outTexCoord;
layout(location = 2) out vec3 outFragPos;

layout(set = 0, binding = 0) uniform FrameBuffer 
{
	mat4 myToView;
	mat4 myProjection;
	vec3 myCameraPosition;
};

layout(set = 1, binding = 0) uniform ObjectBuffer
{
	mat4 myToWorld;
};

void main()
{
	gl_Position = myProjection * myToView * myToWorld * inPosition;
	outNormal = mat3(myToWorld) * inNormal.xyz;
	//outNormal = normalize(mat3(transpose(inverse(myToWorld))) * inNormal.xyz);
	outFragPos = vec3(myToWorld * inPosition);
	outTexCoord = inTexCoords[0];
}