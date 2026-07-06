#version 460
#define OUTPUT_GBUFFER
#include "../common_frag.glsl"

void main() {
	UNPACK_PUSH_CONSTANTS

	vec3 normal     = getNormalFromMap(material, inUV);
	vec3 viewNormal = normalize(mat3(frame.view) * normal);

	outAlbedo = sampleTex(material.albedoIndex, material.samplerIndex, inUV);
	outNormal = vec4(viewNormal, 0.0);
	outORM    = sampleTex(material.ormIndex, material.samplerIndex, inUV);
}