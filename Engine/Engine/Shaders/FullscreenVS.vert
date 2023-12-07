#version 450

 vec4 pos[3] =
{
	vec4(-1.0f, 3.0f, 0.0f, 1.0f),
	vec4(3.0f, -1.0f, 0.0f, 1.0f),
    vec4(-1.0f, -1.0f, 0.0f, 1.0f),
};

vec2 uv[3] =
{
	vec2(0.0f, 1.0f),
	vec2(2.0f, -1.0f),
    vec2(0.0f, -1.0f),
};

layout(location = 1) out vec2 outTexCoord;

void main()
{
	outTexCoord = uv[gl_VertexIndex];
	gl_Position = pos[gl_VertexIndex];
}