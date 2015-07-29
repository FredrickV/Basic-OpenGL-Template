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

class ModelObject : public OpenGLMemoryModel
{
public:
	ModelObject(class Model *model);
	~ModelObject(void);

	void bind();

	void pos(const glm::vec3 &position);
	void scale(const glm::vec3 &scale);
	void rotate(const float &degree, const glm::vec3 &axis);

	

private:
	void update();

	class Model *m_model;
	
	GLuint m_uboHandle;

	glm::vec3 m_position;
	glm::vec3 m_scale;
	float m_rotDegree;
	glm::vec3 m_rotAxis;

	ModelUBO m_uboModel;

	bool m_dirty;
};
