#pragma once
#include <fstream>
#include <sstream>
#include <string>
#include "glm.hpp"
#include "gl_core_4_5.h"
#include "ext.hpp"
#include "glfw3.h"

#include <iostream>

using uint = unsigned int;

class ShaderLoader
{
public:
	// Program ID
	uint ID = 0;

	ShaderLoader(const char* vertex_path, const char* frag_path);
	// Use/activate shader
	void use();
	// utility uniform functions
	uint GetID();
	
private:
	uint vertex_ID = 0;
	uint fragment_ID = 0;

	std::ifstream vertex_file;
	std::ifstream fragment_file;

	std::stringstream vertex_stream;
	std::stringstream fragment_stream;

	std::string vertex_data;
	std::string fragment_data;

	GLint success = GL_FALSE;

	const char* data = nullptr;
};

