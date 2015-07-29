/****************************************************************
* Name:       xxx
* Purpose:    xxx
* Author:     Fredrick Vamstad
* Created:    2015 - June
* Copyright:  Fredrick Vamstad
* Dependency: xxx
****************************************************************/

#include "rendererbase.h"
#include "modelobject.h"
#include "shader.h"
#include "camera.h"

void CreateCube(const float& size, std::vector<glm::vec3>& verticesList,
	std::vector<glm::vec3>& normalsList,
	std::vector<glm::vec2>& tcList,
	std::vector<GLushort>& indices) 
{
	float halfSize = size / 2.0f;
	glm::vec3 positions[8];
	positions[0] = glm::vec3(-halfSize, -halfSize, -halfSize);
	positions[1] = glm::vec3(halfSize, -halfSize, -halfSize);
	positions[2] = glm::vec3(halfSize, halfSize, -halfSize);
	positions[3] = glm::vec3(-halfSize, halfSize, -halfSize);
	positions[4] = glm::vec3(-halfSize, -halfSize, halfSize);
	positions[5] = glm::vec3(halfSize, -halfSize, halfSize);
	positions[6] = glm::vec3(halfSize, halfSize, halfSize);
	positions[7] = glm::vec3(-halfSize, halfSize, halfSize);

	glm::vec3 normals[6];
	normals[0] = glm::vec3(-1.0, 0.0, 0.0);
	normals[1] = glm::vec3(1.0, 0.0, 0.0);
	normals[2] = glm::vec3(0.0, 1.0, 0.0);
	normals[3] = glm::vec3(0.0, -1.0, 0.0);
	normals[4] = glm::vec3(0.0, 0.0, 1.0);
	normals[5] = glm::vec3(0.0, 0.0, -1.0);

	indices.resize(36);
	verticesList.resize(36);
	normalsList.resize(36);
	tcList.resize(36);

	//fill indices array
	GLushort* id = &indices[0];
	//left face
	*id++ = 7; 	*id++ = 3; 	*id++ = 4;
	*id++ = 3; 	*id++ = 0; 	*id++ = 4;

	//right face
	*id++ = 2; 	*id++ = 6; 	*id++ = 1;
	*id++ = 6; 	*id++ = 5; 	*id++ = 1;

	//top face
	*id++ = 7; 	*id++ = 6; 	*id++ = 3;
	*id++ = 6; 	*id++ = 2; 	*id++ = 3;
	//bottom face
	*id++ = 0; 	*id++ = 1; 	*id++ = 4;
	*id++ = 1; 	*id++ = 5; 	*id++ = 4;

	//front face
	*id++ = 6; 	*id++ = 4; 	*id++ = 5;
	*id++ = 6; 	*id++ = 7; 	*id++ = 4;
	//back face
	*id++ = 0; 	*id++ = 2; 	*id++ = 1;
	*id++ = 0; 	*id++ = 3; 	*id++ = 2;

	for (int i = 0;i < 36;i++) {
		int normal_index = i / 6;
		int tc_index = i / 12;
		verticesList[i] = positions[indices[i]];
		normalsList[i] = normals[normal_index];
		//tcList[i] = tc[tc_index];
		indices[i] = i;
	}
}

RendererBase::RendererBase(SDL_Window *sdlWindow) : m_sdlWindow(sdlWindow)
{
	m_vboQUAD = -1;
	m_windowWidth = 0;
	m_windowHeight = 0;

	m_effectFinalTextID = -1;
	m_effectFinalFBO = -1;

	m_shaderPostFX = nullptr;
	m_camera = nullptr;

	m_cameraViewUniform = -1;
	m_cameraProjUniform = -1;
}

RendererBase::~RendererBase(void)
{
	if (m_shaderPostFX) {
		delete m_shaderPostFX;
	}
	if (m_camera) {
		delete m_camera;
	}
}

void RendererBase::resize(const int &width, 
						  const int &height)
{
	m_camera->updateProjection(75.0f, 0.1f, 1000.0f, (float)width / (float)height);
}

bool RendererBase::initializeBase()
{
	bool initialized = false;

	m_camera = new Camera;

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_LINE_SMOOTH);
	glLineWidth(1.5f);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	m_vboQUAD = GenerateQuad(-1, -1, 1, 1);

	if (m_modelCube.load("models/cube.model") && 
		m_modelTeapot.load("models/teapot.model")) {
		m_shaderPostFX = new Shader;
		if (m_shaderPostFX->compileShaderFile("shaders/postfx.vs", Shader::TypeVertex) &&
			m_shaderPostFX->compileShaderFile("shaders/postfx.fs", Shader::TypeFragment) && 
			m_shaderPostFX->linkAndFinalize()) {
			initialized = true;
		}
	}

	if (initialized) {
		loadScene();
	}
	return initialized;
}

void RendererBase::destroy()
{
	for (std::vector<class ModelObject*>::iterator 
		it = m_modelObjects.begin(); it != m_modelObjects.end();
		it++) {
		if ((*it)) {
			delete (*it);
		}
	}
	m_modelObjects.clear();
}

void RendererBase::logicUpdate()
{
	if (m_camera->active()) {

		static int cpos[2];
		// Get mouse move amount
		if (SDL_GetMouseState(&cpos[0], &cpos[1]) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
			m_camera->update(cpos[0], cpos[1]);

			// Set back to original position
			m_camera->mouseOrigin(cpos[0], cpos[1]);
			SDL_WarpMouseInWindow(m_sdlWindow, cpos[0], cpos[1]);
		}

		const Uint8 *keyPresses = SDL_GetKeyboardState(nullptr);

		if (keyPresses[SDL_SCANCODE_W]) {
			m_camera->setMovement(0);
		}
		if (keyPresses[SDL_SCANCODE_S]) {
			m_camera->setMovement(1);
		}
		if (keyPresses[SDL_SCANCODE_A]) {
			m_camera->setMovement(2);
		}
		if (keyPresses[SDL_SCANCODE_D]) {
			m_camera->setMovement(3);
		}
	}
}

void RendererBase::loadScene()
{
	m_camera->setPosDir(glm::vec3(0.0f, 5.0f, 0.0f), glm::vec2(TO_RADIANS(175.0f), 0.0f));


	ModelObject *ground = new ModelObject(&m_modelCube);
	ground->scale(glm::vec3(200.0f, 1.0f, 200.0f));
	ground->pos(glm::vec3(0.0f, 0.0f, 0.0f));

	m_modelObjects.push_back(ground);

	int index = 0;
	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			LightPoint *lp = new LightPoint;
			lp->position = glm::vec3(-10.0f + (x*  12 ), 5.0f, -20.0f + (y*  12 ));

			ModelObject *teapot = new ModelObject(&m_modelTeapot);
			teapot->scale(glm::vec3(20.0f, 20.0f, 20.0f));
			teapot->pos(glm::vec3(lp->position.x-1.0f,
								  0.5f,
								  lp->position.z));

			m_pointLights.push_back(lp);

			m_modelObjects.push_back(teapot);
		}
	}
	
	// Textures
	m_textureGround.load("Ground_SmoothRocks_1k_d.tga", 16);
	m_textureGround.setTextTile(50.0f);
	m_textureGround.setKd(glm::vec3(1.0f));
	m_textureGround.setShininess(100.0f);
	m_textureGround.setSpec(0.1f);

	m_textureTeapot.load("Metal_WallPanel_1k_d.tga", 16);
	m_textureTeapot.setTextTile(4.0f);
	m_textureTeapot.setKd(glm::vec3(1.0f));
	m_textureTeapot.setShininess(100.0f);
	m_textureTeapot.setSpec(2.7f);

	
	m_textureColorTerrain.setKd(glm::vec3(0.0f, 0.0f, 1.0f));
	m_textureColorTerrain.setShininess(100.0f);
	m_textureColorTerrain.setSpec(2.7f);

	std::vector<glm::vec3> verticesList;
	std::vector<glm::vec3> normalsList;
	std::vector<glm::vec2> tcList;
	std::vector<GLushort> indices;

	CreateCube(1, verticesList, normalsList, tcList, indices);
	m_terrainCube.fillVertecies(verticesList, normalsList, tcList, indices);

	m_terrainCube.pos(glm::vec3(0.0f, 1.0f, 0.0f));
	m_terrainCube.scale(glm::vec3(4.0f));
}