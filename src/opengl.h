/****************************************************************
* Name:       xxx
* Purpose:    xxx
* Author:     Fredrick Vamstad
* Created:    2015 - June
* Copyright:  Fredrick Vamstad
* Dependency: xxx
****************************************************************/

#ifndef OPENGL_H_
#define OPENGL_H_

#define SDL_MAIN_HANDLED
#include <SDL.h>

#include <GL/glew.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/projection.hpp>


#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <stdarg.h>
#include <ctime>

#define GLSL_VEC4 __declspec(align(16)) glm::vec4
#define GLSL_VEC3 __declspec(align(16)) glm::vec3
#define GLSL_VEC2 __declspec(align(8))  glm::vec2
#define GLSL_MAT4 __declspec(align(16)) glm::mat4
#define GLSL_BOOL __declspec(align(4))	unsigned int

#define PI 3.141592653589793f
#define HALFPI 1.57079632679489661923f
#define TWOPI 6.2831853071795862f
#define TWOPI_F 6.2831853ff
#define TO_RADIANS(x) (x * 0.017453292519943295f)
#define TO_DEGREES(x) (x * 57.29577951308232f)

#define COLOR_WHITE glm::vec3(1.0f, 1.0f, 1.0f)
#define COLOR_RED glm::vec3(1.0f, 0.0f, 0.0f)
#define COLOR_GREEN glm::vec3(0.0f, 1.0f, 0.0f)
#define COLOR_CYAN glm::vec3(0.0f, 1.0f, 1.0f)
#define COLOR_BLUE glm::vec3(0.0f, 0.0f, 1.0f)
#define COLOR_RGB(r, g, b) ((float)r/255.0f, (float)g/255.0f, (float)b/255.0f)


#ifdef _WIN32
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "../libs/SDL2-2.0.3/lib/x86/SDL2.lib")
#else // Linux, Mac....

#endif

struct OpenGLMemoryModel {
	static void* operator new(size_t size) {
		return _aligned_malloc(size, 16);
	}
	static void operator delete(void* memory) {
		_aligned_free(memory);
	}
};

struct ModelUBO : public OpenGLMemoryModel {
	GLSL_MAT4 modelMatrice;
};

struct LightPoint : public OpenGLMemoryModel {
	GLSL_VEC3 pointViewPosition;
	GLint pointViewPositionUniform;
	GLSL_VEC3 position; // World space
};

struct LightDirectionalUBO : public OpenGLMemoryModel {
	GLSL_VEC3 directionalColor;
	float directionalIntensity;
};

void Log(const char *str, ...);
char *FileRead(const std::string &filePath);
GLuint GenerateQuad(const float afLowerLeftX, const float afLowerLeftY, 
					const float afUpperRightX, const float afUpperRightY);

#endif // OPENGL_H_