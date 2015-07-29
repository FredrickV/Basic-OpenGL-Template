/****************************************************************
* Name:       xxx
* Purpose:    xxx
* Author:     Fredrick Vamstad
* Created:    2015 - June
* Copyright:  Fredrick Vamstad
* Dependency: xxx
****************************************************************/

#include "model.h"

typedef unsigned char byte;
typedef unsigned int uint;
typedef unsigned short ushort;
typedef unsigned long ulong;
typedef char TextStringShort[28];
typedef char TextString[56];
typedef char TextStringLong[128];
typedef char TextStringVLong[256];


static const short MDL_VERSION  = 2;
static const uint MDL_HEADER_SIZE =
	sizeof(MDL_VERSION) +	// Version size
	sizeof(TextStringShort) + // Title text
	sizeof(TextStringShort) + // Author text
	sizeof(unsigned int) +	  // Import mesh flags
	sizeof(unsigned char) +	  // Mesh type
	sizeof(unsigned char) +	  // Animated boolean
	sizeof(uint);			  // Groups count

inline unsigned long ReadMem(void *dst, const void *src, const size_t size)
{
	memcpy(dst, src, size);
	return size;

}
Model::Model(void)
{
	m_bufferHandle = NULL;
	m_totalIndices = NULL;
	vaoArrayHandle = nullptr;
	m_hasTangents = false;
	m_totalVaoHandles = 0;
}

Model::~Model(void)
{
}

bool Model::load(const std::string &modelPath)
{
	Log("Loading model \"%s\"...", modelPath.c_str());

	FILE *fp = fopen(modelPath.c_str(), "rb");
	if (fp) {
		fseek(fp, 0, SEEK_END);
		size_t modelSize = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		char *mdlData = nullptr;
		if (modelSize > MDL_HEADER_SIZE) {
			mdlData = new char[modelSize];
			fread(mdlData, modelSize, 1, fp);
		}
		fclose(fp);

		if (mdlData) {
			unsigned int memPos = 0;
			short version = 0;
			memPos += ReadMem(&version, mdlData, sizeof(version));

			if (version == MDL_VERSION) {
				// Skip title & author
				memPos += sizeof(TextStringShort) * 2;
				// Skip mesh import flags
				memPos += sizeof(unsigned int);
				// Skip mesh type
				memPos += sizeof(unsigned char);
				// Skip "is animated"
				memPos += sizeof(unsigned char);

				uint groups = 0;
				memPos += ReadMem(&groups, (const char *)mdlData + memPos, sizeof(groups));
				if (groups) {
					Log("Groups: %d", groups);
					
					for (uint i = 0; i < groups; i++) {
						group_import_t groupImport;
						// Indexed vertices
						uint verticesSize = 0;
						memPos += ReadMem(&verticesSize, (const char *)mdlData + memPos, sizeof(verticesSize));
						groupImport.indexed_vertices.reserve(verticesSize);
						// Indexed uvs
						uint uvsSize = 0;
						memPos += ReadMem(&uvsSize, (const char *)mdlData + memPos, sizeof(uvsSize));
						groupImport.indexed_uvs.reserve(uvsSize);
						// Indexed normals
						uint normalsSize = 0;
						memPos += ReadMem(&normalsSize, (const char *)mdlData + memPos, sizeof(normalsSize));
						groupImport.indexed_normals.reserve(normalsSize);
						// Indexed tangents
						uint tangentsSize = 0;
						memPos += ReadMem(&tangentsSize, (const char *)mdlData + memPos, sizeof(tangentsSize));
						groupImport.indexed_tangents.reserve(tangentsSize);

						m_hasTangents = tangentsSize > 0;

						// Indexed bitangents
						uint bitangentsSize = 0;
						memPos += ReadMem(&bitangentsSize, (const char *)mdlData + memPos, sizeof(bitangentsSize));
						groupImport.indexed_bitangents.reserve(bitangentsSize);
						// Indexed indices
						uint indicesSize = 0;
						memPos += ReadMem(&indicesSize, (const char *)mdlData + memPos, sizeof(indicesSize));
						groupImport.indices.reserve(indicesSize);

						// Group name
						short nameLen = 0;
						memPos += ReadMem(&nameLen, (const char *)mdlData + memPos, sizeof(nameLen));
						char name[56] = "";
						memPos += ReadMem(&name, (const char *)mdlData + memPos, nameLen);
						groupImport.groupName.append(name);
						// 					// Material name
						nameLen = 0;
						memPos += ReadMem(&nameLen, (const char *)mdlData + memPos, sizeof(nameLen));
						memset(name, 0x00, sizeof(name));
						memPos += ReadMem(&name, (const char *)mdlData + memPos, nameLen);
						groupImport.materialName.append(name);

						glm::vec4 vec4Dat;
						glm::vec3 vec3Dat;
						glm::vec2 vec2Dat;

						// Read vertices
						for (uint x = 0; x < verticesSize; x++) {
							memPos += ReadMem(&vec3Dat.x, (const char *)mdlData + memPos, sizeof(vec3Dat.x));
							memPos += ReadMem(&vec3Dat.y, (const char *)mdlData + memPos, sizeof(vec3Dat.y));
							memPos += ReadMem(&vec3Dat.z, (const char *)mdlData + memPos, sizeof(vec3Dat.y));

							groupImport.indexed_vertices.push_back(vec3Dat);
						}
						for (uint x = 0; x < uvsSize; x++) {
							memPos += ReadMem(&vec2Dat.x, (const char *)mdlData + memPos, sizeof(vec2Dat.x));
							memPos += ReadMem(&vec2Dat.y, (const char *)mdlData + memPos, sizeof(vec2Dat.y));

							groupImport.indexed_uvs.push_back(vec2Dat);
						}
						for (uint x = 0; x < normalsSize; x++) {
							memPos += ReadMem(&vec3Dat.x, (const char *)mdlData + memPos, sizeof(vec3Dat.x));
							memPos += ReadMem(&vec3Dat.y, (const char *)mdlData + memPos, sizeof(vec3Dat.y));
							memPos += ReadMem(&vec3Dat.z, (const char *)mdlData + memPos, sizeof(vec3Dat.y));

							groupImport.indexed_normals.push_back(vec3Dat);
						}
						for (uint x = 0; x < tangentsSize; x++) {
							memPos += ReadMem(&vec4Dat.x, (const char *)mdlData + memPos, sizeof(vec4Dat.x));
							memPos += ReadMem(&vec4Dat.y, (const char *)mdlData + memPos, sizeof(vec4Dat.y));
							memPos += ReadMem(&vec4Dat.z, (const char *)mdlData + memPos, sizeof(vec4Dat.z));
							memPos += ReadMem(&vec4Dat.w, (const char *)mdlData + memPos, sizeof(vec4Dat.w));

							groupImport.indexed_tangents.push_back(vec4Dat);
						}
						for (uint x = 0; x < bitangentsSize; x++) {
							memPos += ReadMem(&vec3Dat.x, (const char *)mdlData + memPos, sizeof(vec3Dat.x));
							memPos += ReadMem(&vec3Dat.y, (const char *)mdlData + memPos, sizeof(vec3Dat.y));
							memPos += ReadMem(&vec3Dat.z, (const char *)mdlData + memPos, sizeof(vec3Dat.z));

							groupImport.indexed_bitangents.push_back(vec3Dat);
						}

						for (uint x = 0; x < indicesSize; x++) {
							unsigned short indices = 0;
							memPos += ReadMem(&indices, (const char *)mdlData + memPos, sizeof(indices));

							groupImport.indices.push_back(indices);
						}
						m_groups.push_back(groupImport);
					}
				}
			}
			else {
				Log("Failed to model\"%s\", version error", modelPath.c_str());
			}
			delete [] mdlData;
		}
	}
	else {
		Log("Failed to open model file \"%s\"", modelPath.c_str());
	}

	if (m_groups.size()) {
		loadVBO();
	}

	return m_groups.size() > 0;
}

void Model::render()
{
	for (unsigned short i = 0; i < m_totalVaoHandles; i++) {
		glBindVertexArray(vaoArrayHandle[i]);
		glDrawElements(GL_TRIANGLES, m_totalIndices[i], GL_UNSIGNED_SHORT, (void*)0);
	}
}

void Model::loadVBO()
{
	m_totalVaoHandles = (unsigned short)m_groups.size();
	vaoArrayHandle = new unsigned int[m_totalVaoHandles];
	glGenVertexArrays( m_groups.size(), vaoArrayHandle);

	int bufferCount = 4;
	if (m_hasTangents) {
		bufferCount += 1;
	}

	m_bufferHandle = new unsigned int[bufferCount * m_groups.size()];
	glGenBuffers(bufferCount * m_groups.size(), m_bufferHandle);

	m_totalIndices = new unsigned short[m_groups.size()];

	int handleIndex = 0;
	int vaoArrayIndex = 0;
	for (std::vector<group_import_t>::iterator it = m_groups.begin(); it != m_groups.end(); it++) {
		group_import_t *group = &(*it);

		glBindVertexArray(vaoArrayHandle[vaoArrayIndex]);

		glEnableVertexAttribArray(0);  // Vertex position
		glBindBuffer(GL_ARRAY_BUFFER, m_bufferHandle[handleIndex++]);
		glBufferData(GL_ARRAY_BUFFER, group->indexed_vertices.size() * sizeof(glm::vec3),  &group->indexed_vertices[0], GL_STATIC_DRAW);
		glVertexAttribPointer( (GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)) );

		glEnableVertexAttribArray(1);  // Vertex normal
		glBindBuffer(GL_ARRAY_BUFFER, m_bufferHandle[handleIndex++]);
		glBufferData(GL_ARRAY_BUFFER, group->indexed_normals.size() * sizeof(glm::vec3), &group->indexed_normals[0], GL_STATIC_DRAW);
		glVertexAttribPointer( (GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)) );

		glEnableVertexAttribArray(2);  // texture coords
		glBindBuffer(GL_ARRAY_BUFFER, m_bufferHandle[handleIndex++]);
		glBufferData(GL_ARRAY_BUFFER, group->indexed_uvs.size() * sizeof(glm::vec2),  &group->indexed_uvs[0], GL_STATIC_DRAW);
		glVertexAttribPointer( (GLuint)2, 2, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)) );

		if (m_hasTangents) {
			glEnableVertexAttribArray(3);  // tangent
			glBindBuffer(GL_ARRAY_BUFFER, m_bufferHandle[handleIndex++]);
			glBufferData(GL_ARRAY_BUFFER, group->indexed_tangents.size() * sizeof(glm::vec4), &group->indexed_tangents[0], GL_STATIC_DRAW);
			glVertexAttribPointer((GLuint)3, 4, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)));
		}

		// 		glEnableVertexAttribArray(4);  // bitangent
		// 		glBindBuffer(GL_ARRAY_BUFFER, m_bufferHandle[handleIndex++]);
		// 		glBufferData(GL_ARRAY_BUFFER, group->indexed_tangents.size() * sizeof(glm::vec4), &group->indexed_tangents[0], GL_STATIC_DRAW);
		// 		glVertexAttribPointer((GLuint)4, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferHandle[handleIndex++]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, group->indices.size() * sizeof(unsigned short), &group->indices[0], GL_STATIC_DRAW);
		m_totalIndices[vaoArrayIndex++] = group->indices.size() ;
	}

	// Clear loaded data
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
}