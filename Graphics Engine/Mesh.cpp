#include "Mesh.h"

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
}

void Mesh::initialiseQuad() {
	// Check if the mesh has been initialised
	assert(vao == 0);

	// Generate Buffers
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);

	// Bind vertex array or the mesh wrapper
	glBindVertexArray(vao);

	// bind vartex buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
}