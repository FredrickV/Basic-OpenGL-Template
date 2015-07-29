/****************************************************************
* Name:       xxx
* Purpose:    xxx
* Author:     Fredrick Vamstad
* Created:    2015 - June
* Copyright:  Fredrick Vamstad
* Dependency: xxx
****************************************************************/

#include "shader.h"


static GLenum GLShaderType(const Shader::Type &type)
{
	switch (type) {
		case Shader::TypeVertex:
			return GL_VERTEX_SHADER;
		case Shader::TypeFragment:	
			return GL_FRAGMENT_SHADER;
		case Shader::TypeGeometry:	
			return GL_GEOMETRY_SHADER;
		case Shader::TypeTC:			
			return GL_TESS_CONTROL_SHADER;
		case Shader::TypeTE:			
			return GL_TESS_EVALUATION_SHADER;
		case Shader::TypeEOL:
		default:
		return GL_NONE;
	}
	return GL_NONE;
}

Shader::Shader(void)
{
	m_programHandle = 0;
	m_shaderValid = false;

	memset(&m_shaderHandles, 0x00, sizeof(m_shaderHandles));
}

Shader::~Shader(void)
{
	destroy();
}

bool Shader::compileShaderFile(const std::string &file, 
							   const Shader::Type &type)
{
	bool compiled = false;
	m_shaderValid = false;

	Log("Compiling shader \"%s\"...", file.c_str());

	if (GLShaderType(type) != GL_NONE) {
		if (m_programHandle < 1) {
			m_programHandle = glCreateProgram();
			if (m_programHandle == 0) {
				Log("Unable to create shader program");
			}
		} 

		char *fd = FileRead(file);
		if (fd) {
			m_shadersStringRaw[type] = std::string(fd);
			delete [] fd;

			if (m_shaderHandles[type] == 0) {
				m_shaderHandles[type] = glCreateShader(GLShaderType(type));

				const char * c_code = m_shadersStringRaw[type].c_str();
				glShaderSource(m_shaderHandles[type], 1, &c_code, NULL );
				glCompileShader(m_shaderHandles[type]);

				int result = 0;
				glGetShaderiv(m_shaderHandles[type], GL_COMPILE_STATUS, &result );
				if( GL_FALSE == result ) {
					Log("Error(s) while compiling shader:\n\n");
					int length = 0;
					glGetShaderiv(m_shaderHandles[type], GL_INFO_LOG_LENGTH, &length );
					if( length > 0 ) {
						char * c_log = new char[length];
						int written = 0;
						glGetShaderInfoLog(m_shaderHandles[type], length, &written, c_log);
						Log(c_log);
						delete [] c_log;
					}
				}
				else {
					Log("Compiled successfully");
					glAttachShader(m_programHandle, m_shaderHandles[type]);
					compiled = true;
				}
			}
			else {
				Log("This shader program supports only a single shader type at the same time");
			}
		}
		else {
			Log("Unable to compile shader, file \"%s\" not found", file.c_str());
		}
	}
	else {
		Log("Shader type \"%d\" not known", type);
	}
	return compiled;
}

bool Shader::linkAndFinalize()
{
	bool finalized = false;

	Log("Linking...");

	if (m_programHandle > 0) {
		// Minimum requirements: Vertex, Fragment. Optional: Geom, ES, EC
		if (m_shaderHandles[TypeVertex] > 0 && m_shaderHandles[TypeFragment] > 0) {
			glLinkProgram(m_programHandle);

			int status = 0;
			glGetProgramiv( m_programHandle, GL_LINK_STATUS, &status);
			if( GL_FALSE == status ) {
				Log("Error(s) while linking shader:\n\n");
				int length = 0;

				glGetProgramiv(m_programHandle, GL_INFO_LOG_LENGTH, &length );
				if( length > 0 ) {
					char * c_log = new char[length];
					int written = 0;
					glGetProgramInfoLog(m_programHandle, length, &written, c_log);
					Log(c_log);
					delete [] c_log;
				}
			}
			else {
				Log("Shader linked and finalized");
				finalized = true;
				m_shaderValid = true;
			}
		}
		else {
			Log("Unable to finalize shader, vertex and fragment shaders missing");
		}
	}
	else {
		Log("Unable to finalize shader, shader program has not been created");
	}

	return finalized;
}

void Shader::destroy()
{
	for (int i = 0; i < TypeEOL; i++) {
		if (m_shaderHandles[i] > 0) {
			glDeleteShader(m_shaderHandles[i]);
			m_shaderHandles[i] = 0;
		}
	}
	if (m_programHandle > 0) {
		glDeleteProgram(m_programHandle);
	}
	m_shaderValid = false;
}

void Shader::use()
{
	glUseProgram( m_programHandle );
}

GLuint Shader::getSubroutine(GLenum type, const char *subroutineName)
{
	return glGetSubroutineIndex(m_programHandle, type, subroutineName);
}

int Shader::getUniform(const char *name) {
	int loc = getUniformLocation(name);
	if( loc == -1 ) {
		Log("Uniform: %s not found.\n", name);
	}
	return loc;
}

void Shader::setUniform( const char *name, const glm::vec3 & v)
{
	int loc = getUniformLocation(name);
	if( loc >= 0 ) {
		glUniform3f(loc,v.x,v.y,v.z);
	} else {
		Log("Uniform: %s not found.\n",name);
	}
}

void Shader::setUniform( const char *name, const glm::vec4 & v)
{
	int loc = getUniformLocation(name);
	if( loc >= 0 ) {
		glUniform4f(loc,v.x,v.y,v.z,v.w);
	} else {
		Log("Uniform: %s not found.\n",name);
	}
}

void Shader::setUniform( const char *name, const glm::vec2 & v)
{
	int loc = getUniformLocation(name);
	if( loc >= 0 ) {
		glUniform2f(loc,v.x,v.y);
	} else {
		Log("Uniform: %s not found.\n",name);
	}
}

void Shader::setUniform( const char *name, const glm::mat4 & m)
{
	int loc = getUniformLocation(name);
	if( loc >= 0 ) {
		glUniformMatrix4fv(loc, 1, GL_FALSE, &m[0][0]);
	} else {
		Log("Uniform: %s not found.\n",name);
	}
}

void Shader::setUniform( const char *name, const glm::mat3 & m)
{
	int loc = getUniformLocation(name);
	if( loc >= 0 ) {
		glUniformMatrix3fv(loc, 1, GL_FALSE, &m[0][0]);
	} else {
		Log("Uniform: %s not found.\n",name);
	}
}

void Shader::setUniform( const char *name, float val )
{
	int loc = getUniformLocation(name);
	if( loc >= 0 ) {
		glUniform1f(loc, val);
	} else {
		Log("Uniform: %s not found.\n",name);
	}
}

void Shader::setUniform( const char *name, int val )
{
	int loc = getUniformLocation(name);
	if( loc >= 0 ) {
		glUniform1i(loc, val);
	} else {
		Log("Uniform: %s not found.\n",name);
	}
}


void Shader::setUniform( const char *name, bool val )
{
	int loc = getUniformLocation(name);
	if( loc >= 0 ) {
		glUniform1i(loc, val);
	} else {
		Log("Uniform: %s not found.\n",name);
	}
}

int Shader::getUniformLocation(const char * name )
{
	return glGetUniformLocation(m_programHandle, name);
}