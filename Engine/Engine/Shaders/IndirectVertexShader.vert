#version 460
#include "MeshStructs.hpp"

layout(location = 0) in vec3 inPosition;
layout(location = 1) in int inColor;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in vec3 inTangent;
layout(location = 4) in vec3 inBinormals;
layout(location = 5) in vec2 inTexCoords[2];

layout(location = 0) out vec3 outNormal;
layout(location = 1) out vec2 outTexCoord;
layout(location = 2) out vec3 outFragPos;
layout(location = 3) out int outDrawID;
layout(location = 4) out vec3 outTangent;
layout(location = 5) out vec3 outBinormals;
layout(location = 6) out vec3 outTangentViewDir;

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
	gl_Position = myProjection * myToView * drawData.myToWorld * vec4(inPosition, 1.0);
	
	//outNormal = normalize(mat3(transpose(inverse(drawData.myToWorld))) * inNormal.xyz);
	
	mat3 toWorldRotation = transpose(inverse(mat3(drawData.myToWorld)));
	outNormal = toWorldRotation * inNormal;
	outTangent = toWorldRotation * inTangent;
	outBinormals = toWorldRotation * inBinormals;
	outFragPos = vec3(drawData.myToWorld * vec4(inPosition, 1.0));
	outTexCoord = inTexCoords[0];
	outDrawID = gl_DrawID;

	vec3 T = normalize(outTangent);
	vec3 N = normalize(outNormal);
	vec3 B = cross(N, T);
	mat3 TBN = transpose(mat3(T, B, N));
	outTangentViewDir = TBN * (myCameraPosition - outFragPos);
}