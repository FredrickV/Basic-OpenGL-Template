/****************************************************************
* Name:       xxx
* Purpose:    xxx
* Author:     Fredrick Vamstad
* Created:    2015 - June
* Copyright:  Fredrick Vamstad
* Dependency: xxx
****************************************************************/

#include "rendererforward.h"
#include "shader.h"
#include "modelobject.h"
#include "camera.h"

RendererForward::RendererForward(SDL_Window *sdlWindow) : RendererBase(sdlWindow)
{
	m_shaderModel = nullptr;
	m_pointLightuboHandle = -1;
}

RendererForward::~RendererForward(void)
{
	if (m_shaderModel) {
		delete m_shaderModel;
	}
}

bool RendererForward::initialize()
{
	bool initialized = false;

	if (initializeBase()) {
		m_shaderModel = new Shader;

		glEnable(GL_DEPTH_TEST);

		if (m_shaderModel->compileShaderFile("shaders/forward_model.vs", Shader::TypeVertex) &&
			m_shaderModel->compileShaderFile("shaders/forward_model.fs", Shader::TypeFragment) && 
			m_shaderModel->linkAndFinalize()) {

			m_shaderModel->use();
			m_cameraProjUniform = m_shaderModel->getUniform("projection_matrix");
			m_cameraViewUniform = m_shaderModel->getUniform("view_matrix");
			
			m_shaderSubTexture = m_shaderModel->getSubroutine(GL_FRAGMENT_SHADER, "TexturedPass");
			m_shaderSubColor = m_shaderModel->getSubroutine(GL_FRAGMENT_SHADER, "ColorPass");

			char buffer[56] = "";
			for (int i = 0; i < 64; i++) {
				sprintf(buffer, "Light[%d].pointViewPosition", i);
				m_pointLights[i]->pointViewPositionUniform = m_shaderModel->getUniform(buffer);
			}
			initialized = true;
		}
	}
	return initialized;
}

void RendererForward::resize(const int &width, 
							 const int &height)
{
	m_windowWidth = width;
	m_windowHeight = height;

	RendererBase::resize(m_windowWidth, m_windowHeight);

	glViewport(0, 0, m_windowWidth, m_windowHeight);

	// Update static uniforms
	m_shaderModel->use();
	m_shaderModel->setUniform(m_cameraProjUniform, m_camera->proj());
}

void RendererForward::render()
{
	// PASS Geometry + Light
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_shaderModel->use();
	glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &m_shaderSubTexture);
	m_shaderModel->setUniform(m_cameraViewUniform, m_camera->view());


	for (auto & i : m_pointLights) {
		m_shaderModel->setUniform(i->pointViewPositionUniform, 
			glm::vec3(m_camera->view() * glm::vec4(i->position, 1.0f)));
	}

	for (std::vector<ModelObject*>::iterator it = m_modelObjects.begin();
		it != m_modelObjects.end(); it++) {
		if (it == m_modelObjects.begin()) {
			m_textureGround.bind();
		}
		else {
			m_textureTeapot.bind();
		}
		(*it)->bind();
	}

	glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &m_shaderSubColor); // Bob will never call this

	m_textureColorTerrain.bind(); // TerrainTextureDraw(obj);
	m_terrainCube.draw(); // VertexBatchDraw(obj);
}


void RendererForward::sdlWindowEvent(const SDL_Event &event)
{
	switch (event.window.event) {
	case SDL_WINDOWEVENT_NONE:
		break;
	case SDL_WINDOWEVENT_SHOWN:
		break;
	case SDL_WINDOWEVENT_HIDDEN:
		break;
	case SDL_WINDOWEVENT_EXPOSED:
		break;
	case SDL_WINDOWEVENT_MOVED:
		break;
	case SDL_WINDOWEVENT_RESIZED:
		resize(static_cast<int>(event.window.data1),
			static_cast<int>(event.window.data2));
		break;
	case SDL_WINDOWEVENT_SIZE_CHANGED:
		break;
	case SDL_WINDOWEVENT_MINIMIZED:
		break;
	case SDL_WINDOWEVENT_MAXIMIZED:
		break;
	case SDL_WINDOWEVENT_RESTORED:
		break;
	case SDL_WINDOWEVENT_ENTER:
		break;
	case SDL_WINDOWEVENT_LEAVE:
		break;
	case SDL_WINDOWEVENT_FOCUS_GAINED:
		break;
	case SDL_WINDOWEVENT_FOCUS_LOST:
		break;
	case SDL_WINDOWEVENT_CLOSE:
		break;
	}
}

void RendererForward::sdlMouseKeyEvent(const Uint32 type,
									   const SDL_Event &event)
{
	switch (type) {
	case SDL_MOUSEBUTTONDOWN:
		if (event.button.button == SDL_BUTTON_LEFT) {
			SDL_ShowCursor(0);

			static int cpos[2];
			SDL_GetMouseState(&cpos[0], &cpos[1]);
			m_camera->setMouseOrigin(cpos[0], cpos[1]);
			m_camera->active() = true;
		}
		break;
	case SDL_MOUSEBUTTONUP:
		if (event.button.button == SDL_BUTTON_LEFT) {
			SDL_ShowCursor(1);

			m_camera->active() = false;
		}
		break;
	case SDL_MOUSEWHEEL:
		break;
	}
	
}

void RendererForward::logicUpdate()
{
	RendererBase::logicUpdate();
}