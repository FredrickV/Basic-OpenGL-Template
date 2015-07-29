/****************************************************************
* Name:       xxx
* Purpose:    xxx
* Author:     Fredrick Vamstad
* Created:    2015 - June
* Copyright:  Fredrick Vamstad
* Dependency: xxx
****************************************************************/

#include "textureterrain.h"
#define  STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#undef STB_IMAGE_IMPLEMENTATION

TextureTerrain::TextureTerrain()
{
	m_texture.uboUniform = -1;
	m_texture.uboData.textTile = 1.0f;
	m_texture.uboData.kd = glm::vec3(1.0f);
	m_texture.uboData.shininess = 50.0f;
	m_texture.uboData.spec = 1.0f;
	m_texture.textID = -1;

	m_hasTexture = false;

	glGenBuffers(1, &m_texture.uboUniform);
	glBindBuffer(GL_UNIFORM_BUFFER, m_texture.uboUniform);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(texture_terrain_t::text_ubo_t),
		&m_texture.uboData, GL_STREAM_DRAW);
}

TextureTerrain::~TextureTerrain()
{
	if (m_texture.uboUniform > 0) {
		glDeleteBuffers(1, &m_texture.uboUniform);
	}
	if (m_texture.textID > 0) {
		glDeleteTextures(1, &m_texture.textID);
	}
}

bool TextureTerrain::load(const std::string &texturePath,
				   const GLint anisotropicFiltering,
				   const bool gammaCorrect)
{
	bool loaded = false;

	int x, y, comp;
	// Hardcoded to 24 bit per pixel (comp = 3)
	auto imageData = stbi_load(texturePath.c_str(), &x, &y, &comp, 3);
	if (imageData) {

		glGenTextures(1, &m_texture.textID);
		glBindTexture(GL_TEXTURE_2D, m_texture.textID);

		if (gammaCorrect) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, x, y, 0,
				GL_RGB, GL_UNSIGNED_BYTE, imageData);
		}
		else {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0,
				GL_RGB, GL_UNSIGNED_BYTE, imageData);
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int asf = anisotropicFiltering;
		if (asf > 16) // or hardware limit
		{
			asf = 16;
		}

		if (asf > 0) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, asf);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}
		// Reset state to NULL texture
		glBindTexture(GL_TEXTURE_2D, 0);

		stbi_image_free(imageData);
		
		loaded = true;
	}
	m_hasTexture = loaded;
	return loaded;
}

void TextureTerrain::bind()
{
	glBindBufferBase(GL_UNIFORM_BUFFER, 2, m_texture.uboUniform);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(texture_terrain_t::text_ubo_t), &m_texture.uboData);

	if (m_hasTexture) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_texture.textID);
	}
}

void TextureTerrain::bindNull()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}