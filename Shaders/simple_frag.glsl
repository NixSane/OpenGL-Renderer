#version 450

in vec3 v_normal;

uniform vec3 light_direction;

uniform sampler2D diffuse_texture;
uniform float time;
in vec2 final_texture_coordinates;

out vec4 final_color;

void main()
{
	vec3 norm = normalize(v_normal);
	vec3 light = normalize(light_direction);
	
	// Calculate lambert term (negate light direction)
	float lambert_term = max(0,min(1,dot(norm, -light)));
	
	// final_color = texture(diffuse_texture, final_texture_coordinates);
	final_color = vec4(lambert_term, lambert_term, lambert_term, 1);
	final_color = vec4(light_direction, 1);
}