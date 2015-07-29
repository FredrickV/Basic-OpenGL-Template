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
#include "model.h"
#include "textureterrain.h"
#include "vertexterrain.h"

class RendererBase : public OpenGLMemoryModel
{

public:
	explicit RendererBase(SDL_Window *sdlWindow);
	virtual ~RendererBase(void);

	virtual bool initialize() = 0;
	virtual void resize(const int &width,
		const int &height);
	virtual void render() = 0;
	virtual void sdlWindowEvent(const SDL_Event &event) = 0;
	virtual void sdlMouseKeyEvent(const Uint32 type,
		const SDL_Event &event) = 0;

	bool initializeBase();

	void destroy();
	virtual void logicUpdate();

	inline class Camera *camera() {
		return m_camera;
	}

protected:

	void loadScene();

	GLuint m_vboQUAD;
	int m_windowWidth;
	int m_windowHeight;

	Model m_modelCube;
	Model m_modelTeapot;

	class Shader *m_shaderPostFX;

	std::vector<class ModelObject*> m_modelObjects;

	std::vector<LightPoint *> m_pointLights;

	// Textures
	TextureTerrain m_textureGround;
	TextureTerrain m_textureTeapot;
	TextureTerrain m_textureColorTerrain;

	// Cube example (for terrain)
	VertexTerrain m_terrainCube;

	// PostFX
	GLuint m_effectFinalTextID;
	GLuint m_effectFinalFBO;

	class Camera *m_camera;

	// Camera uniforms
	GLuint m_cameraViewUniform;
	GLuint m_cameraProjUniform;

	// Shader subroutines
	GLuint m_shaderSubTexture;
	GLuint m_shaderSubColor;

	// SDL
	SDL_Window *m_sdlWindow;
};

