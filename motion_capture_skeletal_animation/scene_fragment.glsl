#version 330 core

out vec4 color;

in vec2 tex_coord;

uniform sampler2D diffuse_sampler;

void main()
{
	color = texture(diffuse_sampler, tex_coord);
}