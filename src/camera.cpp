/****************************************************************
* Name:       xxx
* Purpose:    xxx
* Author:     Fredrick Vamstad
* Created:    2015 - June
* Copyright:  Fredrick Vamstad
* Dependency: xxx
****************************************************************/

#include "camera.h"


Camera::Camera(void)
{
	m_view = glm::mat4(1.0f);
	m_cameraActive = false;
	m_mouseOriginX = -1;
	m_mouseOriginY = -1;
	memset(&m_movement, 0x00, sizeof(m_movement));
}


Camera::~Camera(void)
{
}

void Camera::updateProjection(const float &fov, 
							  const float &near,
							  const float &far,
							  const float &aspect)
{
	m_projection = glm::perspective(fov, aspect, near, far);
}

void Camera::update(int xNew, int yNew)
{
	static bool mouseInverted = false;
	static float mouseSpeed = 0.003f;
	static float movementSpeed = 1.0f;

	// Compute new orientation
	m_rotation.x += mouseSpeed * float(m_mouseOriginX - xNew );
	if (mouseInverted) {
		m_rotation.y -= mouseSpeed * float(m_mouseOriginY - yNew );
	}
	else {
		m_rotation.y += mouseSpeed * float(m_mouseOriginY - yNew );
	}

	// Clamp to 180 degrees (Negative 90 To  Positive 90)
	if (TO_DEGREES(m_rotation.y ) > 90.0f) {
		m_rotation.y = (float)TO_RADIANS(90.0f);
	}
	else if (TO_DEGREES(m_rotation.y ) < -90.0f) {
		m_rotation.y = (float)TO_RADIANS(-90.0f);
	}

	// Clamp to 0 - 360 vertical degrees
	if (TO_DEGREES(m_rotation.x) > 360.0f) {
		m_rotation.x = 0.0f;
	}
	else if (TO_DEGREES(m_rotation.x) < -360.0f) {
		m_rotation.x  = (float)TO_RADIANS(360.0f);
	}

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	m_direction = glm::vec3(
		cos(m_rotation.y) * sin(m_rotation.x),
		sin(m_rotation.y),
		cos(m_rotation.y) * cos(m_rotation.x)
		);


	// Right vector
	glm::vec3 right = glm::vec3(
		sin(m_rotation.x - PI / 2.0f),
		0,
		cos(m_rotation.x - PI / 2.0f)
		);

	if (m_movement[0]) {
		m_position += m_direction * 1.0f * movementSpeed;
	}
	if (m_movement[1]) {
		m_position -= m_direction * 1.0f * movementSpeed;
	}
	if (m_movement[2]) {
		m_position -= right * 1.0f * movementSpeed;
	}
	if (m_movement[3]) {
		m_position += right * 1.0f * movementSpeed;
	}

	// Up vector
	glm::vec3 up = glm::cross(right, m_direction);

	m_view = glm::lookAt(
		m_position,           // Camera is here
		// and looks here : at the same position, plus "direction"
		m_position + m_direction,
		// Head is up (set to 0,-1,0 to look upside-down)
		up
		);

	memset(&m_movement, 0x00, sizeof(m_movement));
}

void Camera::setPosDir(const glm::vec3 &pos,
					   const glm::vec2 &dir)
{
	m_position = pos;
	m_rotation = dir;

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	m_direction = glm::vec3(
		cos(dir.y) * sin(dir.x),
		sin(dir.y),
		cos(dir.y) * cos(dir.x)
		);

	// Right vector
	glm::vec3 right = glm::vec3(
		sin(m_rotation.x - PI / 2.0f),
		0,
		cos(m_rotation.x - PI / 2.0f)
		);

	// Up vector
	glm::vec3 up = glm::cross(right, m_direction);

	m_view = glm::lookAt(
		m_position,           // Camera is here
		// and looks here : at the same position, plus "direction"
		m_position + m_direction,
		// Head is up (set to 0,-1,0 to look upside-down)
		up
		);
}