#include <iostream>
#include "glm.hpp"
#include "gl_core_4_5.h"
#include "ext.hpp"
#include "glfw3.h"

#include "FlyCamera.h"
#include "ShaderLoader.h"
#include "OBJMesh.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using uint = unsigned int;

FlyCamera my_camera = FlyCamera(glm::vec3(10, 10, 10), glm::vec3(0.0f, 0.0f, 0.0f));

float deltaTime = 0.0f, lastframe = 0.0f;
float currentFrame;

struct Vertex
{
	glm::vec3 position;
	glm::vec2 uv;
};

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

	Vertex vertices_better[] =
	{
		/*  Position  */			/*  UV    */	
		glm::vec3(-0.5f, 0.5f, 0.0f),  glm::vec2(1.0f,1.0f), // top right
		glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec2(1.0f,0.0f), // bottom right
		glm::vec3(0.5f, -0.5f, 0.0f),  glm::vec2(0.0f,0.0f), // bottom left
		glm::vec3(0.5f, 0.5f, 0.0f),   glm::vec2(0.0f,1.0f) // top left
	};

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

	/*** CREATE AND 'LOAD' MESH ***/ //
	uint VAO;
	uint VBO;
	uint IBO;
	
	/** Generate IDs to access the objects **/
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &IBO);

	/** Bind the Objects and buffers that are going to be drawn **/
	/** And allocate the geometry and construction data in the current object being drawn **/
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(glm::vec3), &vertices_better[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index_buffer), index_buffer, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
	/*glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)(1 *sizeof(glm::vec3)));*/

	// Reset once everything is allocated
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	/*** Texture Buffer ***/

	uint m_texture;
	int x, y, n;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("..\\Dependencies\\Textures\\Chicken_Texture.jpg", &x, &y, &n, 0);

	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // GL_LINEAR SAMPLE texels
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // GL_NEARESTS RETURNS just closest pixel

	
	stbi_image_free(data);

	// glBindTexture(GL_TEXTURE_2D, m_texture);


	/*** Texture Buffer ***/

	glm::mat4 model = glm::mat4(1.0f);

	// Load Shaders
	ShaderLoader myShader("..\\Shaders\\simple_vertex.glsl",
		"..\\Shaders\\simple_frag.glsl");

	// Load OBJ
	aie::OBJMesh my_object;
	my_object.load("..\\stanford\\Mech_Blockout.obj", false, false);

	glPolygonMode(GL_FRONT, GL_LINE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
//	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

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

		/*model = glm::rotate(model, 0.0f, glm::vec3(0, 1, 0));*/
		my_object.draw(false);

		my_camera.update(deltaTime);

		glm::vec4 color = glm::vec4(0.9f, 0.5f, 0.5f, 0.5f);

		myShader.use();
		auto uniform_location = glGetUniformLocation(myShader.GetID(), "projection_view_matrix");
		glUniformMatrix4fv(uniform_location, 1, false, glm::value_ptr(my_camera.getProjectionView()));
		uniform_location = glGetUniformLocation(myShader.GetID(), "model_matrix");
		glUniformMatrix4fv(uniform_location, 1, false, glm::value_ptr(model));
		uniform_location = glGetUniformLocation(myShader.GetID(), "final_color");
		// glUniform4fv(uniform_location,1, glm::value_ptr(color));

		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, number_of_verts);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// We won't need these anymore
	glDeleteBuffers(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 0;
}