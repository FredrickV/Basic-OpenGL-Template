/****************************************************************
* Name:       xxx
* Purpose:    xxx
* Author:     Fredrick Vamstad
* Created:    2015 - June
* Copyright:  Fredrick Vamstad
* Dependency: xxx
****************************************************************/

#pragma once
#include "rendererbase.h"
class RendererForward :
	public RendererBase
{
public:
	RendererForward(SDL_Window *sdlWindow);
	~RendererForward(void);


	bool initialize();
	void resize(const int &width, 
				const int &height);
	void render();
	void sdlWindowEvent(const SDL_Event &event);
	void sdlMouseKeyEvent(const Uint32 type,
						const SDL_Event &event);

protected:
	void logicUpdate();

private:
	class Shader *m_shaderModel;

	GLuint m_pointLightuboHandle;
};

