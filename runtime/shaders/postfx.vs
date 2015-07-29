/****************************************************************
* Name:       postfx.vs
* Purpose:    
* Author:     Fredrick Vamstad
* Created:    2013
* Dependency: GLSL 4.x
****************************************************************/

#version 420

layout (location = 0) in vec3 VertexPosition;
layout (location = 2) in vec2 VertexTexCoord;

out vec2 UV;

void main()
{
    UV = VertexTexCoord;
	gl_Position = vec4(VertexPosition,1.0);
}