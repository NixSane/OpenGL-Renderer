#version 450

uniform sampler2D diffuse_texture;
uniform float time;
in vec2 final_texture_coordinates;
out vec4 final_color;

void main()
{
	final_color = texture(diffuse_texture, final_texture_coordinates);
}