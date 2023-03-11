#version 450

vec2 positions[3] = vec2[](
	vec2(-0.5, 0.5),
	vec2(0.5, 0.5),
	vec2(0.0, -0.5)
);

vec3 colors[3] = vec3[](
	vec3(1.0, 0.0, 0.0),
	vec3(0.0, 1.0, 0.0),
	vec3(0.0, 0.0, 1.0)
);

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
	vec4 objectPos = vec4(positions[gl_VertexIndex] * 10, 0, 1.0);
	vec4 worldPos = objectPos * myToWorld;
	vec4 viewPos = worldPos * myToView;
	vec4 projPos = viewPos * myProjection;
	gl_Position = projPos;
	fragColor = colors[gl_VertexIndex];
}