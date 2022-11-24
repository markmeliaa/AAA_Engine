#version 430
layout(location = 0) in vec3 my_vertex_position;
layout(location = 1) in vec3 vertex_color0;
layout(location = 2) in vec2 vertex_uv0;

layout(location = 1) uniform mat4 model;
layout(location = 2) uniform mat4 view;
layout(location = 3) uniform mat4 proj;

out vec3 color0;
out vec2 uv0;

void main()
{
	gl_Position = proj*view*model*vec4(my_vertex_position, 1.0);
	color0 = vertex_color0;
	uv0 = vertex_uv0;
}