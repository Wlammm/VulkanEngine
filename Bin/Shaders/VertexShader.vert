#version 460

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

layout(set = 0, binding = 0) uniform FrameBuffer 
{
	mat4 myToView;
	mat4 myProjection;
	vec3 myCameraPosition;
};

layout( push_constant ) uniform constants
{
	mat4 myToWorld;
	int myAlbedoIndex;
	int myNormalIndex;
	int myMaterialIndex;
} inPushConstants;

void main()
{
	gl_Position = myProjection * myToView * inPushConstants.myToWorld * vec4(inPosition, 1.0);
	outNormal = normalize(mat3(transpose(inverse(inPushConstants.myToWorld))) * inNormal.xyz);
	outFragPos = vec3(inPushConstants.myToWorld * vec4(inPosition, 1.0));
	outTexCoord = inTexCoords[0];
	outDrawID = gl_DrawID;
}