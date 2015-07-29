/****************************************************************
* Name:       xxx
* Purpose:    xxx
* Author:     Fredrick Vamstad
* Created:    2015 - June
* Copyright:  Fredrick Vamstad
* Dependency: xxx
****************************************************************/

#include "opengl.h"



void Log(const char *str, ...)
{
	static char buffer[4096];
	va_list args;
	va_start (args, str);
	vsprintf (buffer,str, args);
	va_end (args);

	printf("%s\n", buffer);
}

char *FileRead(const std::string &filePath)
{
	char *fd = nullptr;

	FILE *fp = fopen(filePath.c_str(), "rb");
	if (fp) {
		fseek(fp, 0, SEEK_END);
		size_t fSize = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		if (fSize) {
			fd = new char[fSize +1];
			fread(fd, fSize, 1, fp);
			fd[fSize] = '\0';
		}
		fclose(fp);
	}
	return fd;
}


GLuint GenerateQuad(const float afLowerLeftX, const float afLowerLeftY, 
					const float afUpperRightX, const float afUpperRightY)
{
	GLuint vbo = -1;

	unsigned int handle[2];
	glGenBuffers(2, handle);

	const float afVertexData[] = { afLowerLeftX, afLowerLeftY, 0.0, afUpperRightX, afLowerLeftY, 0.0,
		afLowerLeftX, afUpperRightY, 0.0, afUpperRightX, afUpperRightY, 0.0 };

	const float afTexCoordData[] = { 0, 0, 1, 0, 0, 1, 1, 1 };

	glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
	glBufferData(GL_ARRAY_BUFFER, 4 * 3 * sizeof(float), afVertexData, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
	glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), afTexCoordData, GL_STATIC_DRAW);

	// Set up the vertex array object
	glGenVertexArrays(1, &vbo);
	glBindVertexArray(vbo);

	glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)));
	glEnableVertexAttribArray(0);  // Vertex position

	glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
	glVertexAttribPointer((GLuint)1, 2, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)));
	glEnableVertexAttribArray(1);  // Texture coordinates

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	return vbo;
}