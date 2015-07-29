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

class Model
{
	typedef struct{
		std::string groupName;
		std::string materialName;

		std::vector<unsigned short> indices;

		std::vector<glm::vec3> indexed_vertices;
		std::vector<glm::vec2> indexed_uvs;
		std::vector<glm::vec3> indexed_normals;
		std::vector<glm::vec4> indexed_tangents;
		std::vector<glm::vec3> indexed_bitangents;
	}group_import_t;

public:
	Model(void);
	~Model(void);

	bool load(const std::string &modelPath);
	void render();

private:
	void loadVBO();

	std::vector<group_import_t> m_groups;
	unsigned int *vaoArrayHandle;
	unsigned short *m_totalIndices;
	unsigned short m_totalVaoHandles;
	unsigned int *m_bufferHandle;

	bool m_hasTangents;
};

