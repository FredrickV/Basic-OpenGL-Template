/****************************************************************
* Name:       xxx
* Purpose:    xxx
* Author:     Fredrick Vamstad
* Created:    2015 - June
* Copyright:  Fredrick Vamstad
* Dependency: xxx
****************************************************************/

#include "vertexterrain.h"

#ifndef offsetof
#ifdef _WIN64
#define offsetof(s,m)   (size_t)( (ptrdiff_t)&reinterpret_cast<const volatile char&>((((s *)0)->m)) )
#else  /* _WIN64 */
#define offsetof(s,m)   (size_t)&reinterpret_cast<const volatile char&>((((s *)0)->m))
#endif  /* _WIN64 */
#endif

VertexTerrain::VertexTerrain()
{
	m_vao = -1;
	for (int i = 0; i < 4; i++)
		m_vbo[i] = -1;
	m_vboIndices = -1;
	m_elements = 0;

	m_position = glm::vec3(0.0f);
	m_scale = glm::vec3(1.0f);
	m_rotAxis = glm::vec3(1.0f);
	m_rotDegree = 0.0f;

	m_dirty = true;

	glGenBuffers(1, &m_uboHandle);
	glBindBuffer(GL_UNIFORM_BUFFER, m_uboHandle);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(m_uboModel), &m_uboModel, GL_STREAM_DRAW);
}


VertexTerrain::~VertexTerrain()
{
	cleanup();

	if (m_uboHandle > 0) {
		glDeleteBuffers(1, &m_uboHandle);
	}
}


void VertexTerrain::fillVertecies(const std::vector<glm::vec3> &vertecies,
								  const std::vector<glm::vec3> &normals,
								  const std::vector<glm::vec2> &tc,
								  const std::vector<GLushort> &indices)
{
	
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(4, m_vbo);
	
	
	glBindVertexArray(m_vao);
	
	glEnableVertexAttribArray(0);  // Vertex position
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, vertecies.size() * sizeof(glm::vec3), &vertecies[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)));

	glEnableVertexAttribArray(1);  // Vertex normal
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)));

	glEnableVertexAttribArray(2);  // texture coords
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, tc.size() * sizeof(glm::vec2), &tc[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)2, 2, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vbo[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);
	m_elements = static_cast<GLushort>(indices.size());
	
	/*
	cleanup();

	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_vboIndices);
	glBindVertexArray(m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	//pass vertices to the buffer object
	glBufferData(GL_ARRAY_BUFFER, vertecies.size()*sizeof(TerrainVertex), &vertecies[0], GL_STATIC_DRAW);


	//enable vertex attribute array for position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TerrainVertex), 0);

	//enable vertex attribute array for normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(TerrainVertex), (const GLvoid*)(offsetof(TerrainVertex, normal)));

	//enable vertex attribute array for tc
// 	glEnableVertexAttribArray(2);
// 	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(TerrainVertex), (const GLvoid*)(offsetof(TerrainVertex, tc)));

	//pass cube indices to element array buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboIndices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(GLushort), &indices[0], GL_STATIC_DRAW);
*/
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	
	
}

void VertexTerrain::pos(const glm::vec3 &position)
{
	m_position = position;
	m_dirty = true;
}

void VertexTerrain::scale(const glm::vec3 &scale)
{
	m_scale = scale;
	m_dirty = true;
}

void VertexTerrain::rotate(const float &degree, const glm::vec3 &axis)
{
	m_rotDegree = degree;
	m_rotAxis = axis;
	m_dirty = true;
}

void VertexTerrain::update()
{
	m_uboModel.modelMatrice = glm::translate(glm::mat4(1.0f), m_position);
	m_uboModel.modelMatrice *= glm::rotate(m_rotDegree, m_rotAxis);
	m_uboModel.modelMatrice *= glm::scale(m_scale);

	m_dirty = false;
}

void VertexTerrain::draw()
{
	if (m_dirty) {
		update();
	}

	glBindBufferBase(GL_UNIFORM_BUFFER, 1, m_uboHandle);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(m_uboModel), &m_uboModel);

	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, m_elements, GL_UNSIGNED_SHORT, 0);
}

void VertexTerrain::cleanup()
{

}