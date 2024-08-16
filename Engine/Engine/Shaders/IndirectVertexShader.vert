#version 460
#include "MeshStructs.hpp"

layout(location = 0) in vec4 inPosition;
layout(location = 1) in vec4 inColor;
layout(location = 2) in vec4 inNormal;
layout(location = 3) in vec4 inTangent;
layout(location = 4) in vec4 inBinormals;
layout(location = 5) in vec2 inTexCoords[4];

layout(location = 0) out vec3 outNormal;
layout(location = 1) out vec2 outTexCoord;
layout(location = 2) out vec3 outFragPos;
layout(location = 3) out int outDrawID;

layout(set = 0, binding = 0) uniform FrameBuffer 
{
	mat4 myToView;
	mat4 myProjection;
	vec3 myCameraPosition;
};

layout(std430, set = 0, binding = 4) readonly buffer PerDrawDataBuffer
{
	PerDrawData perDrawData[];
} inPerDrawData;


void main()
{
	PerDrawData drawData = inPerDrawData.perDrawData[gl_DrawID];
	gl_Position = myProjection * myToView * drawData.myToWorld * inPosition;
	//outNormal = mat3(myToWorld) * inNormal.xyz;
	outNormal = normalize(mat3(transpose(inverse(drawData.myToWorld))) * inNormal.xyz);
	outFragPos = vec3(drawData.myToWorld * inPosition);
	outTexCoord = inTexCoords[0];
	outDrawID = gl_DrawID;
}