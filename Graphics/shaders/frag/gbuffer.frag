#version 460
#define OUTPUT_GBUFFER
#include "../common_frag.glsl"

void main() {
	UNPACK_PUSH_CONSTANTS

	outAlbedo = sampleTex(material.albedoIndex, material.samplerIndex, inUV);
	outNormal = sampleTex(material.normalIndex, material.samplerIndex, inUV);
	outORM    = sampleTex(material.ormIndex,    material.samplerIndex, inUV);
}