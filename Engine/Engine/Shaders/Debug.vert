#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in int inColor;

layout(location = 0) out int outColor;

layout(set = 0, binding = 0) uniform FrameBuffer 
{
	mat4 myToView;
	mat4 myProjection;
};

void main()
{
	vec4 position = vec4(inPosition, 1.0);
    gl_Position = myProjection * myToView * position;
	outColor = inColor;
}
