#version 450 // Phong vertex shader

layout(location = 0) in vec3 local_position;
layout(location = 1) in vec2 texture_coordinates;
layout(location = 2) in vec4 normal;

uniform mat4 projection_view_matrix;
uniform mat4 model_matrix; 


out vec3 v_normal;
out vec2 final_texture_coordinates;

// Matrix to transform normals
uniform mat3 normal_matrix;

void main()
{
	v_normal = normal_matrix * normal.xyz;
	final_texture_coordinates = texture_coordinates;
	gl_Position = projection_view_matrix * model_matrix * vec4(local_position, 1.0f);
}
