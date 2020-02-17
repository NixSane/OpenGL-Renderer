#include <iostream>
#include "glm.hpp"
#include "gl_core_4_5.h"
#include "ext.hpp"
#include "glfw3.h"

#include "FlyCamera.h"
#include "ShaderLoader.h"
#include "OBJMesh.h"

using uint = unsigned int;

FlyCamera my_camera = FlyCamera(glm::vec3(10, 10, 10), glm::vec3(0.0f, 0.0f, 0.0f));

float deltaTime = 0.0f, lastframe = 0.0f;
float currentFrame;



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

	// Mesh Data
	glm::vec3 vertices[] =
	{
		glm::vec3(-0.5f, 0.5f, 0.0f),
		glm::vec3(-0.5f, -0.5f, 0.0f),
		glm::vec3(0.5f, -0.5f, 0.0f),
		glm::vec3(0.5f, 0.5f, 0.0f),
		glm::vec3(-0.5f, 0.5f, 1.0f),
		glm::vec3(-0.5f, -0.5f, 1.0f),
		glm::vec3(0.5f, -0.5f, 1.0f),
		glm::vec3(0.5f, 0.5f, 1.0f)
		
	};
	//int number_of_verts = 18;
	// Refers to the points of each triangle
	int index_buffer[]{ 
		3,1,0, // First triangle
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
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index_buffer), index_buffer, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

	// Reset once everything is allocated
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glm::mat4 model = glm::mat4(1.0f);

	// Load Shaders
	ShaderLoader myShader("..\\Shaders\\simple_vertex.glsl",
		"..\\Shaders\\simple_frag.glsl");

	// Load OBJ
	aie::OBJMesh my_object;
	my_object.load("..\\stanford\\Mech_Blockout.obj", false, false);

	glPolygonMode(GL_FRONT, GL_LINE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

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

		model = glm::rotate(model, 0.016f, glm::vec3(0, 1, 0));
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