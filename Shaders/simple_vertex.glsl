#version 450 // Phong vertex shader

layout(location = 0) in vec3 local_position; // Object's position
layout(location = 1) in vec4 normal; // Surface normal
layout(location = 2) in vec2 uv; // Tex Coordinates
layout(location = 3) in vec4 Tangent;

uniform mat4 projection_view_matrix; // Camera view pyramid
uniform mat4 model_matrix; // Model local matrix for world coordinates
uniform mat3 normal_matrix; // Matrix to transform normals

out vec4 v_position; // Position of the camera
out vec3 v_normal; // the normals of the model's surfaces
out vec3 v_tangent;
out vec3 v_bi_tangent;
out vec2 final_texture_coordinates; // Texture coordinates

void main()
{
	final_texture_coordinates = uv;
	v_position = model_matrix * vec4(local_position, 1);
	v_normal = normal_matrix * normal.xyz;
	v_tangent = normal_matrix * Tangent.xyz;
	v_bi_tangent = cross(v_normal, v_tangent) * Tangent.w;
	gl_Position = projection_view_matrix * model_matrix * vec4(local_position, 1.0f);
}
