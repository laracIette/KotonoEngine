#version 460
#include "../common.glsl"

layout(location = 0) in vec2 inUV;
layout(location = 1) in vec3 inNormal;

layout(location = 0) out vec4 outColor;

void main() {
    DrawData  d       = pc.drawData.data[pc.drawIndex];
    Material  mat     = pc.materials.data[d.materialIndex];
    Transform trans   = pc.transforms.data[d.transformIndex];
    Parameters params = pc.parameters.data[d.parametersIndex];

    vec4 colorParam = params.vectors[0];

    vec4 albedo    = sampleTex(mat.albedoIndex,    mat.samplerIndex, inUV);
    vec4 normSamp  = sampleTex(mat.normalIndex,    mat.samplerIndex, inUV);
    vec4 roughSamp = sampleTex(mat.roughnessIndex, mat.samplerIndex, inUV);
    
    outColor = albedo * colorParam;
}