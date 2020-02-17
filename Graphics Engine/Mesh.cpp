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

void Mesh::draw()
{
	glBindVertexArray(vao);
	// using indices or just vertices?
	if (ibo != 0)
		glDrawElements(GL_TRIANGLES, 3 * tri_count, GL_UNSIGNED_INT, 0);
	else
		glDrawArrays(GL_TRIANGLES, 0, 3 * tri_count);
}