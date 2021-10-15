out vec4 color;

in vec2 tex_coord;

uniform sampler2D diffuse_sampler;
uniform sampler2D specular_sampler;
uniform sampler2D normal_sampler;
uniform sampler2D alpha_sampler;

void main()
{
#ifdef has_alpha_texture
	float alpha = texture(alpha_sampler, tex_coord).a;
	if (alpha < 0.5)
		discard;
#endif
	color = texture(diffuse_sampler, tex_coord);
}