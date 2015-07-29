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
#include "stb_image.h"

typedef struct : public OpenGLMemoryModel {
	typedef struct  {
		GLSL_VEC3 kd;
		float textTile;
		float shininess;
		float spec;
	}text_ubo_t;

	text_ubo_t uboData;
	GLuint uboUniform;

	GLuint textID;	// Diffuse Texture ID
}texture_terrain_t;

class TextureTerrain
{
public:
	TextureTerrain();
	virtual ~TextureTerrain();

	// Jpeg, PNG, TGA, BMP,... (No DDS)
	bool load(const std::string &texturePath,
			  const GLint anisotropicFiltering,
			  const bool gammaCorrect = false);

	void inline setTextTile(const float textureTiles) {
		m_texture.uboData.textTile = textureTiles;
	}
	void inline setKd(const glm::vec3 &kd) {
		m_texture.uboData.kd = kd;
	}
	void inline setShininess(const float shininess) {
		m_texture.uboData.shininess = shininess;
	}
	void inline setSpec(const float spec) {
		m_texture.uboData.spec = spec;
	}

	const inline float textTile() {
		return m_texture.uboData.textTile;
	}

	void bind();
	static void bindNull();

private:
	bool m_hasTexture;
	texture_terrain_t m_texture;
};

/*
Example of interface

typedef struct {
	typedef void * handle;
	int id;
}TextureHandle;

TextureHandle obj = TerrainTextureCreate();
TerrainTextureLoadDiffuseTex(obj, "");
bool ret = TerrainTextureDelete(obj);
TerrainTextureSetKD(obj, glm::vec3(1.0f));

TerrainTextureDraw(obj);
*/