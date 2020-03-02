#version 450 // Phong frag shader

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	sampler2D normal;
	float shine; // Specular power
};

struct Light {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec4 v_position;
in vec3 v_normal;
in vec2 final_texture_coordinates;
in vec3 v_tangent;
in vec3 v_bi_tangent;


// Normal light
uniform vec3 light_direction;

// Second light
 uniform vec3 second_ambient_light;
 uniform vec3 second_diffuse_light;
 uniform vec3 second_specular_light;
 uniform vec3 second_light_direction;

// Surface materials lighting
uniform float specular_power = 32.0f; // material specular power

uniform Material material;
uniform Light light;

uniform vec3 cameraPosition;

out vec4 final_color;

void main()
{
	// Ensure light is normalized
	vec3 norm = normalize(v_normal);
	vec3 tan = normalize(v_tangent);
	vec3 b_tan = normalize(v_bi_tangent);
	vec3 light_d = normalize(light_direction);
	vec3 second_light = normalize(second_light_direction);
	
	mat3 TBN = mat3(tan,b_tan,norm); 
	
	// texture colour and lighting
	vec3 tex_diffuse = texture( material.diffuse, final_texture_coordinates).rgb;
	vec3 tex_specular = texture( material.specular, final_texture_coordinates).rgb;
	vec3 tex_normal = texture( material.normal, final_texture_coordinates).rgb;
	
	norm = TBN * ((tex_normal * 2) - 1);
	
	// Calculate lambert term (negate light direction)
	float lambert_term = max(0,min(1,dot(norm, -light_d)));
	float second_lambert_term = max(0,min(1,dot(norm, -second_light)));
	
	// Calculate view vector and reflection vector. Or the specular
	vec3 view = normalize(cameraPosition - v_position.xyz);
	vec3 R = reflect(light_d, norm);
	vec3 second_R = reflect(second_light, norm);
	
	// Calculate specular term
	float specular_term = pow( max(0,dot(R, view)), specular_power);
	float second_specular_term = pow( max(0,dot(second_R, view)), specular_power);	
	
	/** First Light **/
	vec3 ambient = light.ambient * tex_diffuse;
	vec3 diffuse = light.diffuse * lambert_term * tex_diffuse;
	vec3 specular = light.specular * specular_term * tex_specular;
	
	/** Second Light **/
	vec3 second_ambient = second_ambient_light * tex_diffuse;
	vec3 second_diffuse = second_lambert_term * tex_diffuse;
	vec3 second_specular = second_specular_light * second_specular_term * tex_specular;
	
	
	final_color = vec4((ambient + diffuse + specular) + (second_ambient + second_diffuse + second_specular), 1);
}