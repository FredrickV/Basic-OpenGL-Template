/****************************************************************
* Name:       xxx
* Purpose:    xxx
* Author:     Fredrick Vamstad
* Created:    2015 - June
* Copyright:  Fredrick Vamstad
* Dependency: xxx
****************************************************************/

#include "modelobject.h"
#include "model.h"

ModelObject::ModelObject(class Model *model) : m_model(model)
{
	m_position = glm::vec3(0.0f);
	m_scale = glm::vec3(1.0f);
	m_rotAxis = glm::vec3(1.0f);
	m_rotDegree = 0.0f;

	m_dirty = true;

	glGenBuffers(1, &m_uboHandle);
	glBindBuffer(GL_UNIFORM_BUFFER, m_uboHandle);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(m_uboModel), &m_uboModel, GL_STREAM_DRAW);
}

ModelObject::~ModelObject(void)
{
	if (m_uboHandle > 0) {
		glDeleteBuffers(1, &m_uboHandle);
	}
}

void ModelObject::bind()
{
	if (m_dirty) {
		update();
	}

	glBindBufferBase(GL_UNIFORM_BUFFER, 1, m_uboHandle);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(m_uboModel), &m_uboModel);

	m_model->render();
}

void ModelObject::pos(const glm::vec3 &position)
{
	m_position = position;
	m_dirty = true;
}

void ModelObject::scale(const glm::vec3 &scale)
{
	m_scale = scale;
	m_dirty = true;
}

void ModelObject::rotate(const float &degree, const glm::vec3 &axis)
{
	m_rotDegree = degree;
	m_rotAxis = axis;
	m_dirty = true;
}

void ModelObject::update()
{
	m_uboModel.modelMatrice = glm::translate(glm::mat4(1.0f), m_position);
	m_uboModel.modelMatrice *= glm::rotate(m_rotDegree, m_rotAxis);
	m_uboModel.modelMatrice *= glm::scale(m_scale);

	m_dirty = false;
}