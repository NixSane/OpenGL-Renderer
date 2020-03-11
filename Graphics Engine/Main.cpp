#include <iostream>
#include "glm.hpp"
#include "gl_core_4_5.h"
#include "ext.hpp"
#include "glfw3.h"

#include "FlyCamera.h"
#include "ShaderLoader.h"
#include "OBJMesh.h"
#include "Mesh.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using uint = unsigned int;

FlyCamera my_camera = FlyCamera(glm::vec3(10, 10, 10), glm::vec3(0.0f, 0.0f, 0.0f));

float deltaTime = 0.0f, lastframe = 0.0f;
float currentFrame;

struct Light
{
	glm::vec3 direction;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

Light m_light;
Light m_light_two;

int main()
{
	/* Initialisation*/
	if (glfwInit() == false)
		return -1;

	GLFWwindow* window = glfwCreateWindow(1280, 720, "Computer Graphics", nullptr, nullptr);

	if (window == nullptr) {
		glfwTerminate();
		return -2;
	}

	glfwMakeContextCurrent(window);

	//Testing OpenGL functions
	if (ogl_LoadFunctions() == ogl_LOAD_FAILED) {
		glfwDestroyWindow(window);
		glfwTerminate();
		return -3;
	}
	// Last mouse point
	/*glfwSetCursorPosCallback(window, mouse_callback);*/
	
	auto major = ogl_GetMajorVersion();
	auto minor = ogl_GetMinorVersion();
	printf("GL: %i.%i\n", major, minor);


	Vertex normal_vertex[4];

	normal_vertex[0].position = glm::vec3(-20.0f, -15.0f, 20.0f);
	normal_vertex[1].position = glm::vec3(20.0f, -15.0f, 20.0f);
	normal_vertex[2].position = glm::vec3(20.0f, -15.0f, -20.0f);
	normal_vertex[3].position = glm::vec3(-20.0f, -15.0f, -20.0f);

	normal_vertex[0].normal = { 0,1,0,0 };
	normal_vertex[1].normal = { 0,1,0,0 };
	normal_vertex[2].normal = { 0,1,0,0 };
	normal_vertex[3].normal = { 0,1,0,0 };

	normal_vertex[0].uv = glm::vec2(1.0f, 1.0f);
	normal_vertex[1].uv = glm::vec2(1.0f, 0.0f);
	normal_vertex[2].uv = glm::vec2(0.0f, 0.0f);
	normal_vertex[3].uv = glm::vec2(0.0f, 1.0f);



	// Mesh Data
	glm::vec3 vertices[] =
	{
		// Positions for verteces	
		glm::vec3(-0.5f, 0.5f, 0.0f),  
		glm::vec3(-0.5f, -0.5f, 0.0f),
		glm::vec3(0.5f, -0.5f, 0.0f),
		glm::vec3(0.5f, 0.5f, 0.0f)	

	/*	glm::vec3(-0.5f, 0.5f, 1.0f),		
		glm::vec3(-0.5f, -0.5f, 1.0f),		
		glm::vec3(0.5f, -0.5f, 1.0f),		
		glm::vec3(0.5f, 0.5f, 1.0f)		*/	
	};
	//int number_of_verts = 18;
	// Refers to the points of each triangle
	int index_buffer[]{ 
		0,1,3, // First triangle
		1,2,3  // Second triangle
		/* And so on~~~ */
	};


	//for (int i = 0; i < 6; i++) 
	//{
	//	vertices[i] = (glm::vec3(pvm * glm::vec4(vertices[i], 1)));
	//}

	///*** CREATE AND 'LOAD' MESH ***/ //
	uint VAO;
	uint VBO;
	uint IBO;
	//
	///** Generate IDs to access the objects **/
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &IBO);

	///** Bind the Objects and buffers that are going to be drawn **/
	///** And allocate the geometry and construction data in the current object being drawn **/
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(Vertex), &normal_vertex[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index_buffer), index_buffer, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(7 * sizeof(float)));

	//// Reset once everything is allocated
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	/*Mesh some_mesh(normal_vertex, index_buffer);*/

	// Light
	m_light.direction = { 1.0f, 1.0f, 1.0f };
	m_light.diffuse = { 1.0f, 0.0f, 0.0f };
	m_light.specular = { 0.5f, 0.0f, 1.0f };
	glm::vec3 ambientLight = { 0.8f, 0.8f, 0.8f };

	glm::vec3 ambientMatLight = { 1.0f, 0.5f, 0.31f };
	glm::vec3 dif_mat_Light = { 1.0f, 0.5f, 0.31f };
	glm::vec3 specular_mat_Light = { 0.5f, 0.5f, 0.5f };

	m_light_two.direction = { 0.1f, 0.5f, 0.5f };
	m_light_two.diffuse = { 0.0f, 0.0f, 1.0f };
	m_light_two.specular = { 0.5f, 0.5f, 0.5f };
	


	/*** Texture Buffer ***/

	uint m_texture;
	int x, y, n;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("..\\Dependencies\\Textures\\UVAlbedoMap_Shield.jpg", &x, &y, &n, 0);

	glGenTextures(1, &m_texture);

	// Set texture slot
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	if (n == 3) 
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}
	else if (n == 4)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // GL_LINEAR SAMPLE texels
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // GL_NEARESTS RETURNS just closest pixel
	
	stbi_image_free(data);

	/*** Texture Buffer ***/


	glm::mat4 model;
	model = glm::mat4(1.0f);

	// Load Shaders
	ShaderLoader myShader("..\\Shaders\\simple_vertex.glsl",
		"..\\Shaders\\simple_frag.glsl");

	// Load OBJ
	aie::OBJMesh obj_one;
	
	aie::OBJMesh obj_two;

	obj_one.load("..\\stanford\\Mech_Blockout.obj", false, false);
	obj_two.load("..\\stanford\\meshSwordShield.obj", false, false);

	//glPolygonMode(GL_FRONT, GL_LINE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	

	// Keep window open until the ESCAPE key is pressed
	while (glfwWindowShouldClose(window) == false 
		&& glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		static int framecount = 0;
		framecount++;

		/* Frame count */
		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastframe;
		lastframe = currentFrame;

		float time = glfwGetTime();

		// rotate light
		// m_light.direction = glm::normalize(glm::vec3(glm::cos(time * 2), glm::sin(time * 2), 0));

		// model = glm::rotate(model, 0.016f, glm::vec3(0, 1, 0));
		obj_one.draw(false);
		obj_two.draw(false);


		glm::vec4 color = glm::vec4(0.9f, 0.5f, 0.5f, 0.5f);

		myShader.use();
		// Camera view
		auto uniform_location = glGetUniformLocation(myShader.GetID(), "projection_view_matrix");
		glUniformMatrix4fv(uniform_location, 1, false, glm::value_ptr(my_camera.getProjectionView()));

		// Send camera position to shader
		uniform_location = glGetUniformLocation(myShader.GetID(), "cameraPosition");
		glUniform3fv(uniform_location, 1, glm::value_ptr(my_camera.getPosition()));

		/*glBindTexture(GL_TEXTURE_2D, m_texture);*/

		// Model, Obj transform
		uniform_location = glGetUniformLocation(myShader.GetID(), "model_matrix");
		glUniformMatrix4fv(uniform_location, 1, false, glm::value_ptr(model[0]));

		/** First light **/

		// Ambient lighting
		uniform_location = glGetUniformLocation(myShader.GetID(), "light.ambient");
		glUniform3fv(uniform_location, 1, glm::value_ptr(ambientLight));

		// Diffuse lighting
		uniform_location = glGetUniformLocation(myShader.GetID(), "light.diffuse");
		glUniform3fv(uniform_location, 1, glm::value_ptr(m_light.diffuse));

		// Specular lighting
		uniform_location = glGetUniformLocation(myShader.GetID(), "light.specular");
		glUniform3fv(uniform_location, 1, glm::value_ptr(m_light.specular));

		/** ~First light~ **/


		/** Second Light **/

		// Diffuse lighting
		uniform_location = glGetUniformLocation(myShader.GetID(), "light_2.diffuse");
		glUniform3fv(uniform_location, 1, glm::value_ptr(m_light_two.diffuse));

		// Specular lighting
		uniform_location = glGetUniformLocation(myShader.GetID(), "light_2.specular");
		glUniform3fv(uniform_location, 1, glm::value_ptr(m_light_two.specular));

		/** ~Second Light~ **/

		// Ambient material lighting
		uniform_location = glGetUniformLocation(myShader.GetID(), "Ka");
		glUniform3fv(uniform_location, 1, glm::value_ptr(ambientMatLight));

		// Diffuse material lighting
		uniform_location = glGetUniformLocation(myShader.GetID(), "Kd");
		glUniform3fv(uniform_location, 1, glm::value_ptr(dif_mat_Light));

		// Ambient material lighting
		uniform_location = glGetUniformLocation(myShader.GetID(), "Ks");
		glUniform3fv(uniform_location, 1, glm::value_ptr(specular_mat_Light));

		// Light Direction drawing
		uniform_location = glGetUniformLocation(myShader.GetID(), "light.direction");
		glUniform3fv(uniform_location, 1, glm::value_ptr(m_light.direction));

		// Seconf light direction
		uniform_location = glGetUniformLocation(myShader.GetID(), "light_2.direction");
		glUniform3fv(uniform_location, 1, glm::value_ptr(m_light_two.direction));

		// Direct lighting
		uniform_location = glGetUniformLocation(myShader.GetID(), "normal_matrix");
		glUniformMatrix3fv(uniform_location, 1, false, glm::value_ptr(glm::inverseTranspose(glm::mat3(model))));
		
		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, number_of_verts);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,0);
		
		my_camera.update(deltaTime);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// We won't need these anymore
	/*some_mesh.~Mesh();*/
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &IBO);

	glfwTerminate();
	return 0;
}