#version 450 // Phong vertex shader

layout(location = 0) in vec3 local_position;
layout(location = 1) in vec4 normal;
layout(location = 2) in vec2 uv;

uniform mat4 projection_view_matrix; // Camera view pyramid
uniform mat4 model_matrix; // Model local matrix for world coordinates


out vec4 v_position; // Position of the camera
out vec3 v_normal; // the normals of the model's surfaces
out vec2 final_texture_coordinates; // Texture coordinates

// Matrix to transform normals
uniform mat3 normal_matrix;

void main()
{
	v_position = model_matrix * vec4(local_position, 1);
	v_normal = normal_matrix * normal.xyz;
	//final_texture_coordinates = texture_coordinates;
	gl_Position = projection_view_matrix * model_matrix * vec4(local_position, 1.0f);
}
