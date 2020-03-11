#version 450 // Phong frag shader

struct Material {
	sampler2D diffuse;
	sampler2D normal;
};

struct Light {
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec4 v_position;
in vec3 v_normal;
in vec2 final_texture_coordinates;
in vec3 v_tangent;
in vec3 v_bi_tangent;


// Surface materials lighting
uniform float specular_power = 32.0f; // material specular power

/* The material/textures */
uniform Material material;

/* Lights */
uniform Light light;
uniform Light light_2;

/* Material Lights */
uniform vec3 Ka; // ambient material colour
uniform vec3 Kd; // diffuse material colour
uniform vec3 Ks; // specular material colour

uniform vec3 cameraPosition;

out vec4 final_color;

void main()
{
	vec4 texture_colour = texture(material.diffuse, final_texture_coordinates);
	
	// Normal
	vec4 normal_colour = texture(material.normal, final_texture_coordinates);
	vec3 texture_normal = normal_colour.xyz * 2.0f - 1.0f;
	
	// Ensure light is normalized
	vec3 norm = normalize(v_normal);
	vec3 tan = normalize(v_tangent);
	vec3 b_tan = normalize(v_bi_tangent);
	
	// Normalise light direction
	vec3 light_d = normalize(light.direction); // First light
	vec3 second_light = normalize(light_2.direction); // Second light
	
	// Normal mapping
	mat3 TBN = mat3(tan,b_tan,norm); 
	
	vec3 mapped_normal = TBN * texture_normal;
	
	// Calculate lambert term (negate light direction)
	float lambert_term = max(0,min(1,dot(mapped_normal, -light_d)));
	float second_lambert_term = max(0,min(1,dot(mapped_normal, -second_light)));
	
	// Calculate view vector and reflection vector. Or the specular
	vec3 view = normalize(cameraPosition - v_position.xyz);
	vec3 R = reflect(light_d, -mapped_normal);
	vec3 second_R = reflect(second_light, -mapped_normal);
	
	// Calculate specular term
	float specular_term = pow( max(0,dot(R, view)), specular_power);
	float second_specular_term = pow( max(0,dot(second_R, view)), specular_power);	
	
	/** First Light **/
	vec3 ambient = light.ambient * Ka;
	vec3 diffuse = light.diffuse * lambert_term * Kd;
	vec3 specular = light.specular * specular_term * Ks;
	
	/** Second Light **/
	vec3 second_ambient = light_2.ambient * Ka;
	vec3 second_diffuse = light_2.diffuse * second_lambert_term * Kd;
	vec3 second_specular = light_2.specular * second_specular_term * Ks;
	
	vec3 result_light = ambient + (diffuse + specular + second_diffuse + second_specular);
	
	
	final_color = texture_colour * vec4(result_light, 1);
}