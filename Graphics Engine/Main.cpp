#include <iostream>
#include "glm.hpp"
#include "gl_core_4_5.h"
#include "ext.hpp"
#include "glfw3.h"

#include "FlyCamera.h"

#include <fstream>
#include <sstream>

using uint = unsigned int;

FlyCamera my_camera = FlyCamera(2.0f);

/*** Camera ***/
glm::vec3 camera_pos = glm::vec3(0.0f, 0.0f, 3.0f); // Where the Camera is
glm::vec3 camera__target = glm::vec3(0.0f, 0.0f, 0.0f); // Just where the camera is focused on
glm::vec3 camera__direction = glm::normalize(camera_pos - camera__direction); // Direction is the opposite of the real direction Camera is facing

glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f); // World Axis Up vector

/** Camera transform **/
glm::vec3 camera_right = glm::normalize(glm::cross(up, camera__direction));
glm::vec3 camera_up = glm::cross(camera__direction, camera_right);
glm::vec3 camera_front = glm::vec3(0.0f, 0.0f, -1.0f);

/** Camera View **/
glm::mat4 view;

float deltaTime = 0.0f, lastframe = 0.0f;
float currentFrame;

/* Rotation */
glm::vec3 direction;
float yaw = -90.0f;
float pitch;

// Camera Input
void processInput(GLFWwindow* window)
{
	const float camera_speed = 2.5f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera_pos += camera_speed * camera_front;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera_pos -= camera_speed * camera_front;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera_pos -= glm::normalize(glm::cross(camera_front, camera_up)) * camera_speed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera_pos += glm::normalize(glm::cross(camera_front, camera_up)) * camera_speed;
}

double mouse_x, mouse_y;
float x_offset, y_offset;
float lastX = 400, lastY = 300;
bool first_mouse = true;

// Mouse Input
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (first_mouse)
	{
		lastX = xpos;
		lastY = ypos;
		first_mouse = false;
	}

	x_offset = xpos - lastX;
	y_offset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	const float sensitivity = 0.05f;
	x_offset *= sensitivity;
	y_offset *= sensitivity;

	yaw += x_offset;
	pitch += y_offset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	camera_front = glm::normalize(direction);
}


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
	glfwSetCursorPosCallback(window, mouse_callback);
	
	auto major = ogl_GetMajorVersion();
	auto minor = ogl_GetMinorVersion();
	printf("GL: %i.%i\n", major, minor);

	// Mesh Data
	glm::vec3 vertices[] =
	{
		glm::vec3(-0.5f, 0.5f, 0.0f),
		glm::vec3(-0.5f, -0.5f, 0.0f),
		glm::vec3(0.5f, -0.5f, 0.0f),
		glm::vec3(0.5f, 0.5f, 0.0f)
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

	/*** CREATE AND 'LOAD' MESH ***/
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

	/* CAMERA */
	glm::mat4 projection = glm::perspective(1.507f, 16 / 9.0f, 0.2f, 50.0f);
	glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 1), glm::vec3(0,0,0), glm::vec3(0, 1, 0));
	glm::mat4 model = glm::mat4(1.0f);

	my_camera.setPosition(camera_pos);
	my_camera.setPerspective(90.0f, 16.0f / 9.0f, 0.2f, 50.0f);

	// glm::mat4 pvm = projection * view * model;

	uint vertex_shader_ID = 0;
	uint fragment_shader_ID = 0;
	uint shader_program_ID = 0;

	/* Vertex Shader */
	// Load Shader
	std::string shader_data;
	std::ifstream in_file_stream("..\\Shaders\\simple_vertex.glsl", std::ifstream::in);

	// Load the source into a string for compilation
	std::stringstream string_stream;
	if (in_file_stream.is_open())
	{
		string_stream << in_file_stream.rdbuf();
		shader_data = string_stream.str();
		in_file_stream.close();
	}
	else
	{
		// File wasn't able to be opened
		throw std::exception("SHADER_SOURCE COULD NOT BE FOUND!");
	}

	// Allocate space for Shader program
	vertex_shader_ID = glCreateShader(GL_VERTEX_SHADER);
	// Convert to raw data
	const char* data = shader_data.c_str();
	// Send in the char* to shader location
	glShaderSource(vertex_shader_ID, 1, (const GLchar**)&data, 0);
	// Build
	glCompileShader(vertex_shader_ID);

	// Did it work?
	// Check the shader compiled
	GLint success = GL_FALSE;
	glGetShaderiv(vertex_shader_ID, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		std::cout << "Vertex Shader Failed\n";
	}
	

	/* Fragment Shader */
	// Load Shader
	//std::string frag_data;
	std::ifstream frag_file_stream("..\\Shaders\\simple_frag.glsl", std::ifstream::in);
	// std::stringstream frag_string_stream;

	// Load the source into a string for compilation
	std::stringstream frag_stream;
	if (frag_file_stream.is_open())
	{
		frag_stream << frag_file_stream.rdbuf();
		shader_data = frag_stream.str();
		frag_file_stream.close();
	}
	else
	{
		// File wasn't able to be opened
		throw std::exception("SHADER_SOURCE COULD NOT BE FOUND!\n");
	}

	// Allocate space for Shader program
	fragment_shader_ID = glCreateShader(GL_FRAGMENT_SHADER);
	// Convert to raw data
	data = shader_data.c_str();
	// Send in the char* to shader location
	glShaderSource(fragment_shader_ID, 1, (const GLchar**)&data, 0);
	// Build
	glCompileShader(fragment_shader_ID);

	// Did it work?
	// Check the shader compiled
	success = GL_FALSE;
	glGetShaderiv(fragment_shader_ID, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		std::cout << "Fragment Shader Failed\n";
	}
	

	// Finally link here
	// Create the new shader program
	shader_program_ID = glCreateProgram();

	// Attach both shaders by ID and type
	glAttachShader(shader_program_ID, vertex_shader_ID);
	glAttachShader(shader_program_ID, fragment_shader_ID);


	glPolygonMode(GL_FRONT, GL_LINE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glLinkProgram(shader_program_ID);

	success = GL_FALSE;
	glGetProgramiv(shader_program_ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		// Get the length of the OpenGL error message
		GLint log_length = 0;
		glGetShaderiv(shader_program_ID, GL_INFO_LOG_LENGTH, &log_length);
		// Create the error buffer
		char* log = new char[log_length];
		// Copy the error from the buffer
		glGetProgramInfoLog(shader_program_ID, log_length, 0, log);

		// Create the error message
		std::string error_message(log);
		error_message += "SHADER_FAILED_TO_COMPILE";
		printf(error_message.c_str());
		// Clean up anyway

		delete[] log;
	}

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

		/** Camera Input **/
		/*processInput(window);*/
	
		/*model = glm::rotate(model, 0.016f, glm::vec3(0, 1, 0));

		view = glm::lookAt(camera_pos, camera_pos + camera_front, camera_up);*/
		my_camera.update(deltaTime);

		glm::mat4 pv = projection * view;

		glm::vec4 color = glm::vec4(0.9f, 0.5f, 0.5f, 0.5f);

		glUseProgram(shader_program_ID);
		auto uniform_location = glGetUniformLocation(shader_program_ID, "projection_view_matrix");
		glUniformMatrix4fv(uniform_location, 1, false, glm::value_ptr(my_camera.getProjectionView()));
		uniform_location = glGetUniformLocation(shader_program_ID, "model_matrix");
		glUniformMatrix4fv(uniform_location, 1, false, glm::value_ptr(my_camera.getWorldTransform()));
		uniform_location = glGetUniformLocation(shader_program_ID, "color");
		glUniform4fv(uniform_location,1, glm::value_ptr(color));

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