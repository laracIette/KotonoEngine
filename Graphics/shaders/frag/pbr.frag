#version 460
#include "../common.glsl"

layout(location = 0) in vec2 inUV;
layout(location = 1) in vec3 inNormal;

layout(location = 0) out vec4 outColor;

void main() {
    UNPACK_PUSH_CONSTANTS_FRAG

    const vec4 albedo    = sampleTex(material.albedoIndex,    material.samplerIndex, inUV);
    const vec4 normSamp  = sampleTex(material.normalIndex,    material.samplerIndex, inUV);
    const vec4 roughSamp = sampleTex(material.roughnessIndex, material.samplerIndex, inUV);
    
    outColor = albedo;
}