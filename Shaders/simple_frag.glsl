#version 450 // Phong frag shader

in vec4 v_position;
in vec3 v_normal;
in vec2 final_texture_coordinates;
in vec3 v_tangent;
in vec3 v_bi_tangent;

// Normal light
uniform vec3 ambient_light;
uniform vec3 diffuse_light;
uniform vec3 specular_light;
uniform vec3 light_direction;

// Surface materials lighting
uniform vec3 ambient_mat_light; // Ambient material lighting
uniform vec3 diffuse_mat_light; // Diffuse material lighting
uniform vec3 specular_mat_light; // specular material lighting
uniform float specular_power = 32.0f; // material specular power

uniform sampler2D diffuse_texture;
uniform sampler2D specular_texture;
uniform sampler2D normal_texture;

uniform vec3 cameraPosition;

out vec4 final_color;

void main()
{
	// Ensure light is normalized
	vec3 norm = normalize(v_normal);
	vec3 tan = normalize(v_tangent);
	vec3 b_tan = normalize(v_bi_tangent);
	vec3 light = normalize(light_direction);
	
	mat3 TBN = mat3(tan,b_tan,norm); 
	
	// texture colour and lighting
	vec3 tex_diffuse = texture( diffuse_texture, final_texture_coordinates).rgb;
	vec3 tex_specular = texture( specular_texture, final_texture_coordinates).rgb;
	vec3 tex_normal = texture( normal_texture, final_texture_coordinates).rgb;
	
	norm = TBN * ((tex_normal * 2) - 1);
	
	// Calculate lambert term (negate light direction)
	float lambert_term = max(0,min(1,dot(norm, -light)));
	
	// Calculate view vector and reflection vector
	vec3 view = normalize(cameraPosition - v_position.xyz);
	vec3 R = reflect(light, norm);
	
	// Calculate specular term
	float specular_term = pow( max(0,dot(R, view)), specular_power);
	
	// Calculate each colour property
	// vec4 texture_colour = texture(diffuse_texture, final_texture_coordinates);
	// vec3 colour_to_add = vec3(texture_colour.x, texture_colour.y, texture_colour.z);
	
	vec3 ambient = ambient_light * ambient_mat_light;
	vec3 diffuse = diffuse_mat_light * diffuse_light * lambert_term;
	vec3 specular = specular_light * specular_mat_light * specular_term;
	
	tex_diffuse *= diffuse;
	tex_normal *= ambient;
	tex_specular *= specular;
	
	final_color = vec4(tex_diffuse + tex_specular + tex_normal, 1);
	// final_color = vec4(tex_normal, 1);
	// final_color = texture(diffuse_texture, final_texture_coordinates);
	//final_color = vec4(lambert_term, lambert_term, lambert_term, 1);
}