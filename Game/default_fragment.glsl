#version 430
out vec4 color;

in vec3 color0;
in vec2 uv0;

layout(binding = 5) uniform sampler2D mytexture;

void main()
{
	color = texture2D(mytexture, uv0) * vec4(color0, 1.0);
}
