#version 420

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 Texcoord;

out VS_OUT
{
    vec3        ws_coords;
    vec3        normal;
    vec2        tcDiff;
} vs_out;


layout(std140, binding=1) uniform
ModelUniform {
	mat4 modelMatrice;
};

layout(std140, binding=2) uniform
MaterialUniform {
	vec3 kd;
	float textTile;
	float shininess;
	float spec;
}Mat;

uniform mat4 projection_matrix;
uniform mat4 view_matrix;

void main()
{
	mat4 modelView = view_matrix * modelMatrice;
	mat3 normalMatrix = mat3(modelView);

    vs_out.ws_coords = (modelView * vec4(Position, 1.0f)).xyz;
    vs_out.normal = normalize(normalMatrix * Normal);
    vs_out.tcDiff = (Texcoord * vec2(1.0, -1.0)) * Mat.textTile;
	
	gl_Position = (projection_matrix * modelView) * vec4(Position, 1.0f);
}