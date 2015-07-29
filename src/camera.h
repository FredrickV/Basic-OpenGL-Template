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

class Camera
{
public:
	Camera(void);
	~Camera(void);

	void updateProjection(const float &fov, 
						  const float &near,
						  const float &far,
						  const float &aspect);
	void update(int xNew, int yNew);
	void setPosDir(const glm::vec3 &pos,
				   const glm::vec2 &dir);

	inline glm::mat4 &proj() {
		return m_projection;
	}
	inline glm::mat4 &view() {
		return m_view;
	}
	inline glm::vec3 &pos() {
		return m_position;
	}
	inline glm::vec2 &rot() {
		return m_rotation;
	}
	inline glm::vec3 &direction() {
		return m_direction;
	}

	inline void setMouseOrigin(const int &x,
							   const int &y) {
		m_mouseOriginX = x;
		m_mouseOriginY = y; 
	}

	inline void mouseOrigin(int &originX,
							int &originY) {
		originX = m_mouseOriginX;
		originY = m_mouseOriginY;
	}

	inline bool &active() {
		return m_cameraActive;
	}

	inline void setMovement(const int &index) {
		m_movement[index] = true;
	}

private:
	glm::vec2 m_rotation;
	glm::vec3 m_position;
	glm::vec3 m_direction;

	glm::mat4 m_view;
	glm::mat4 m_projection;

	int m_mouseOriginX;
	int m_mouseOriginY;

	bool m_cameraActive;

	bool m_movement[4];
};

