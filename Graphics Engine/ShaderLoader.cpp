#include "ShaderLoader.h"

ShaderLoader::ShaderLoader(const char* vertex_path, const char* frag_path)
{
	vertex_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragment_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// Open the files
		vertex_file.open(vertex_path);
		fragment_file.open(frag_path);
		// Read the data into the streams
		vertex_stream << vertex_file.rdbuf();
		fragment_stream << fragment_file.rdbuf();

		// Load the shaders' data into a veriable.
		if (vertex_file.is_open())
		{
			vertex_data = vertex_stream.str();
		}
		if (fragment_file.is_open())
		{
			fragment_data = fragment_stream.str();
		}

		// Close those files
		vertex_file.close();
		fragment_file.close();
	}
	catch(std::ifstream::failure error)
	{
		std::cout << "ERROR:: CAN'T READ SHADER FILE" << std::endl;
	}

	// Compile Vertex Shader
	vertex_ID = glCreateShader(GL_VERTEX_SHADER);
	// Convert to raw data
	const char* data = vertex_data.c_str();
	// Send in the char* to shader location
	glShaderSource(vertex_ID, 1, (const GLchar**)&data, 0);
	// Build
	glCompileShader(vertex_ID);

	glGetShaderiv(vertex_ID, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		std::cout << "Vertex Shader Failed\n";
	}

	// Do the same for the fragment shader

	// Compile fragment Shader
	fragment_ID = glCreateShader(GL_FRAGMENT_SHADER);
	// Convert to raw data
	data = fragment_data.c_str();
	// Send in the char* to shader location
	glShaderSource(fragment_ID, 1, (const GLchar**)&data, 0);
	// Build
	glCompileShader(fragment_ID);

	glGetShaderiv(fragment_ID, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		std::cout << "Fragment Shader Failed\n";
	}

	/** Link them together and make the program **/
	ID = glCreateProgram();

	// Attach the shaders by ID and type of shaders
	glAttachShader(ID, vertex_ID);
	glAttachShader(ID, fragment_ID);
	glLinkProgram(ID);

	// Check if it fails
	success = GL_FALSE;
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		// Get the length of the OpenGL error message
		GLint log_length = 0;
		glGetShaderiv(ID, GL_INFO_LOG_LENGTH, &log_length);
		// Create the error buffer
		char* log = new char[log_length];
		// Copy the error from the buffer
		glGetProgramInfoLog(ID, log_length, 0, log);

		// Create the error message
		std::string error_message(log);
		error_message += "SHADER_FAILED_TO_COMPILE";
		printf(error_message.c_str());
		// Clean up anyway

		delete[] log;
	}

	// Shaders are linked and can be deleted from the program safely
	glDeleteShader(vertex_ID);
	glDeleteShader(fragment_ID);
}

void ShaderLoader::use()
{
	glUseProgram(ID);
}

uint ShaderLoader::GetID()
{
	return ID;
}