#version 450 // Phong frag shader

in vec4 v_position;
in vec3 v_normal;

// Normal light
uniform vec3 ambient_light;
uniform vec3 diffuse_light;
uniform vec3 specular_light;
uniform vec3 light_direction;

// Surface materials lighting
uniform vec3 ambient_mat_light; // Ambient material lighting
uniform vec3 diffuse_mat_light; // Diffuse material lighting
uniform vec3 specular_mat_light; // specular material lighting
uniform float specular_power; // material specular power

uniform sampler2D diffuse_texture;
uniform vec3 cameraPosition;

in vec2 final_texture_coordinates;

out vec4 final_color;

void main()
{
	// Ensure light is normalized
	vec3 norm = normalize(v_normal);
	vec3 light = normalize(light_direction);
	
	// Calculate lambert term (negate light direction)
	float lambert_term = max(0,min(1,dot(norm, -light)));
	
	// Calculate view vector and reflection vector
	vec3 view = normalize(cameraPosition - v_position.xyz);
	vec3 R = reflect(light, norm);
	
	// Calculate specular term
	float specular_term = pow( max(0,dot(R, view)), specular_power);
	
	// Calculate each colour property
	vec3 ambient = ambient_light * ambient_mat_light;
	vec3 diffuse = diffuse_mat_light * diffuse_light * lambert_term;
	vec3 specular = specular_light * specular_mat_light * specular_term;
	
	// final_color = texture(diffuse_texture, final_texture_coordinates);
	//final_color = vec4(lambert_term, lambert_term, lambert_term, 1);
	final_color = vec4(diffuse + ambient + specular, 1);
}