#pragma once
#include "glm.hpp"
#include "gl_core_4_5.h"

#include <iostream>
#include <vector>

using uint = unsigned int;

struct Vertex
{
	glm::vec3 position;
	glm::vec2 uv;
	glm::vec4 normal;
};

class Mesh
{
public:

	Mesh();
	Mesh(Vertex a_vertices[], int a_index[]);
	virtual ~Mesh();

	void initialiseQuad();

	

	GLint getVAO() const;
	GLint getVBO() const;
	GLint getIBO() const;

	

	/*virtual void draw();*/

protected:
	uint vao, vbo, ibo;
};

