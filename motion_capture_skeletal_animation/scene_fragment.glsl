out vec4 color;

in vec2 tex_coord;

uniform sampler2D diffuse_sampler;
uniform sampler2D specular_sampler;
uniform sampler2D normal_sampler;
uniform sampler2D alpha_sampler;

void main()
{
	color = texture(diffuse_sampler, tex_coord);
}