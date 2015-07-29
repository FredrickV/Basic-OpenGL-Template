/****************************************************************
* Name:       xxx
* Purpose:    xxx
* Author:     Fredrick Vamstad
* Created:    2015 - June
* Copyright:  Fredrick Vamstad
* Dependency: xxx
****************************************************************/

#ifndef SHADER_H_
#define SHADER_H_

#include "opengl.h"






class Shader
{	
public:
	enum Type {
		TypeVertex,		// Vertex Shader
		TypeFragment,	// Fragment Shader
		TypeGeometry,	// Geometry Shader
		TypeTC,			// Tessellation Control Shader 
		TypeTE,			// Tessellation Evaluation Shader
		
		TypeEOL
	};
public:
	Shader(void);
	~Shader(void);

	bool compileShaderFile(const std::string &file, 
						   const Shader::Type &type);
	bool linkAndFinalize();
	void destroy();
	bool valid() { return m_shaderValid; }

	void use();
	GLuint getSubroutine(GLenum type, const char *subroutineName);
	int getUniform(const char *name);
	void setUniform( const char *name, const glm::vec2 & v);
	inline void setUniform(const int &loc, const glm::vec2 & v) {
		glUniform2f(loc,v.x,v.y);
	}
	inline void setUniform(int *loc, const glm::vec2 & v) {
		glUniform2f(*loc,v.x,v.y);
	}
	void   setUniform( const char *name, const glm::vec3 & v);
	inline void setUniform(const int &loc, const glm::vec3 & v) {
		glUniform3f(loc,v.x,v.y,v.z);
	}
	inline void setUniform(int *loc, const glm::vec3 & v) {
		glUniform3f(*loc,v.x,v.y,v.z);
	}
	void   setUniform( const char *name, const glm::vec4 & v) ;
	inline void setUniform(const int &loc, const glm::vec4 & v) {
		glUniform4f(loc,v.x,v.y,v.z,v.w);
	}
	inline void setUniform(int *loc, const glm::vec4 & v) {
		glUniform4f(*loc,v.x,v.y,v.z,v.w);
	}
	void   setUniform( const char *name, const glm::mat4 & m);
	inline void setUniform(const int &loc, const glm::mat4 & m) {
		glUniformMatrix4fv(loc, 1, GL_FALSE, &m[0][0]);
	}
	inline void setUniform(const int *loc, const glm::mat4 & m) {
		glUniformMatrix4fv(*loc, 1, GL_FALSE, &m[0][0]);
	}
	void   setUniform( const char *name, const glm::mat3 & m);
	inline void setUniform(const int &loc, const glm::mat3 & m) {
		glUniformMatrix3fv(loc, 1, GL_FALSE, &m[0][0]);
	}
	inline void setUniform(const int *loc, const glm::mat3 & m) {
		glUniformMatrix3fv(*loc, 1, GL_FALSE, &m[0][0]);
	}
	void   setUniform( const char *name, float val );
	inline void setUniform(const int &loc, float val) {
		glUniform1f(loc, val);
	}
	inline void setUniform(const int *loc, const float & v) {
		glUniform1f(*loc, v);
	}
	void   setUniform( const char *name, int val );
	inline void setUniform(const int &loc, int val) {
		glUniform1i(loc, val);
	}
	inline void setUniform(const int *loc, const int & v) {
		glUniform1i(*loc, v);
	}
	void setUniform( const char *name, bool val );
	inline void setUniform(const int &loc, bool val) {
		glUniform1i(loc, val);
	}
	inline void setUniform(const int *loc, const bool & v) {
		glUniform1i(*loc, v);
	}
private:
	int getUniformLocation(const char * name );

	GLuint m_programHandle;
	GLuint m_shaderHandles[TypeEOL];
	std::string m_shadersStringRaw[TypeEOL];
	bool m_shaderValid;
};

#endif // SHADER_H_