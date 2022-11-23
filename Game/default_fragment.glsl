#version 430
out vec4 color;

in vec2 uv0;

layout(binding = 5) uniform sampler2D mytexture;

void main()
{
	//color = vec4(0.925, 0.835, 0.0, 1.0);
	color = texture2D(mytexture, uv0);
}
