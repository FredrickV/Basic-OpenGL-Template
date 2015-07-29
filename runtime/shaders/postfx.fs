/****************************************************************
* Name:       postfx.fs
* Purpose:    
* Author:     Fredrick Vamstad
* Created:    2013
* Dependency: GLSL 4.x
****************************************************************/

#version 420

out vec3 ColorOut;
layout (binding=0) uniform sampler2D RenderedTexture;

in vec2 UV;

void main()
{
	// Gamma correct
	ColorOut = pow(texture2D( RenderedTexture, UV ).xyz,
							  vec3(1.0 / 2.2) );  
}
