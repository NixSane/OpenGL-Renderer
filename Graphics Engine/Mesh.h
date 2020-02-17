#pragma once
#include "glm.hpp"
#include "gl_core_4_5.h"

using uint = unsigned int;

class Mesh
{
public:
	Mesh() : tri_count(0), vao(0), vbo(0), ibo(0) {}
	virtual ~Mesh();

	struct Vertex {
		glm::vec4 position;
		glm::vec4 normal; // Surface face direction
		glm::vec2 tex_coord; // Texture Coordinates
	};

	void initialiseQuad();

	virtual void draw();

protected:
	uint tri_count; // Number of triangles
	uint vao, vbo, ibo;
};

