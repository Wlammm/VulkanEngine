#version 460

layout(location = 0) in vec3 inPosition;
layout(location = 1) in int inColor;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in vec3 inTangent;
layout(location = 4) in vec3 inBinormals;
layout(location = 5) in vec2 inTexCoords[2];

layout(location = 0) out vec2 outTexCoord;
layout(location = 1) out vec3 outModelFragPos;

layout(set = 0, binding = 0) uniform FrameBuffer
{
    mat4 myToView;
    mat4 myProjection;
    vec3 myCameraPosition;
};

void main()
{
    mat3 toViewRotation = transpose(mat3(myToView));
    
    gl_Position = myProjection * vec4(inPosition, 1.0);
    outModelFragPos = toViewRotation * inPosition;
    outTexCoord = inTexCoords[0];
}