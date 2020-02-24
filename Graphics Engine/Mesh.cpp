#include "Mesh.h"


Mesh::Mesh()
{
	// Check if this mesh hasn't been initialised
	if (vao != 1)
	{
		/** Generate IDs to access the objects **/
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ibo);
	}
}

Mesh::Mesh(Vertex a_vertices[], int a_index[])
{
	/** Generate IDs to access the objects **/
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);
	

	/** Bind the Objects and buffers that are going to be drawn **/
	/** And allocate the geometry and construction data in the current object being drawn **/
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex), &a_vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(a_index), a_index, GL_STATIC_DRAW);

	// Position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	// UV
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)8);

	// Normal
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)16);


	// Reset once everything is allocated
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

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

	// Refers to the points of each triangle
	int index_buffer[]{
		3,1,0, // First triangle
		3,2,1  // Second triangle
		/* And so on~~~ */
	};

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index_buffer), index_buffer, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

	// Reset once everything is allocated
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

//void Mesh::draw()
//{
//	glBindVertexArray(vao);
//	// using indices or just vertices?
//	if (ibo != 0)
//		glDrawElements(GL_TRIANGLES, 3 * tri_count, GL_UNSIGNED_INT, 0);
//	else
//		glDrawArrays(GL_TRIANGLES, 0, 3 * tri_count);
//}

GLint Mesh::getVAO() const
{
	return vao;
}

GLint Mesh::getVBO() const
{
	return vbo;
}

GLint Mesh::getIBO() const
{
	return ibo;
}


