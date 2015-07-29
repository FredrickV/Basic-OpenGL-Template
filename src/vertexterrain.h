/****************************************************************
* Name:       xxx
* Purpose:    xxx
* Author:     Fredrick Vamstad
* Created:    2015 - June
* Copyright:  Fredrick Vamstad
* Dependency: xxx
****************************************************************/

#pragma once

#include "opengl.h"

typedef struct {
	glm::vec3 pos, normal;
	glm::vec2 tc;
}TerrainVertex;


class VertexTerrain
{
public:
	VertexTerrain();
	virtual ~VertexTerrain();


	void fillVertecies(const std::vector<glm::vec3> &vertecies,
					   const std::vector<glm::vec3> &normals,
					   const std::vector<glm::vec2> &tc,
					   const std::vector<GLushort> &indices);

	void pos(const glm::vec3 &position);
	void scale(const glm::vec3 &scale);
	void rotate(const float &degree, const glm::vec3 &axis);

	void draw();

private:
	void update();
	void cleanup();

	// Data / Drawing
	GLuint m_vao;
	GLuint m_vbo[4];
	GLuint m_vboIndices;

	GLushort m_elements;

	// Transformation
	GLuint m_uboHandle;

	glm::vec3 m_position;
	glm::vec3 m_scale;
	float m_rotDegree;
	glm::vec3 m_rotAxis;

	ModelUBO m_uboModel;

	bool m_dirty;
};



/*
Example of interface

typedef struct {
typedef void * handle;
int id;
}VertexBatchHandle;

TextureHandle obj = VertexBatchCreate();
VertexBatchSetData(obj, vertecies, normals, tc);
VertexBatchDelete(obj);

VertexBatchMatrix(obj, mat4x4);

VertexBatchDraw(obj);
*/