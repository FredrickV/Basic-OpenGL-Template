#version 420

layout (binding = 0) uniform sampler2D tex_diffuse;
layout (binding = 1) uniform sampler2D tex_normal_map;

subroutine void RenderPassType();
subroutine uniform RenderPassType RenderPass;

out vec3 ColorOut;

in VS_OUT
{
    vec3    ws_coords;
    vec3    normal;
    vec2    tcDiff;
} fs_in;

layout(std140, binding=2) uniform
MaterialUniform {
	vec3 kd;
	float textTile;
	float shininess;
	float spec;
}Mat;

struct LightPoint {
	vec3 pointViewPosition;
};

uniform LightPoint Light[64];

const float LightStrength = 1.5f;
const vec3 LightAmbient = vec3(0.2f);
const vec3 LightKd = vec3(0.9f, 0.9f, 0.4f);
const vec3 LightKs = vec3(0.95f, 0.95f, 0.95f);


vec3 ads(LightPoint light)
{

	float dist = length(light.pointViewPosition - fs_in.ws_coords);
	vec3 s = normalize( light.pointViewPosition - fs_in.ws_coords );
    vec3 v = normalize(vec3(-fs_in.ws_coords));
	
	vec3 normal = normalize(fs_in.normal);
	
    vec3 r = reflect( -s, normal );
	
    return 
        (LightStrength * (LightKd * max(dot(s, normal) , 0.0 ) +
          Mat.spec  * (LightKs * pow( max( dot(r,v), 0.0 ), Mat.shininess )) ) ) / dist;	
}

subroutine (RenderPassType)
void TexturedPass()
{
	vec3 diff = texture(tex_diffuse, fs_in.tcDiff).xyz * Mat.kd;

	for (int i = 0; i < 64; i++) {
		ColorOut += ads(Light[i]);
	}
	ColorOut = (diff * ColorOut) + (diff * LightAmbient);
}

subroutine (RenderPassType)
void ColorPass()
{
	vec3 diff = Mat.kd;

	for (int i = 0; i < 64; i++) {
		ColorOut += ads(Light[i]);
	}
	ColorOut = (diff * ColorOut) + (diff * LightAmbient);

}

void main(void)
{
	RenderPass();
}

