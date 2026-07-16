#version 460
#include "../common_frag.glsl"

layout(location = 0) in vec2 inUV;

layout(location = 0) out vec4 outColor;

void main() 
{
    UNPACK_PUSH_CONSTANTS

    const uint textureParam = parameters.textures[0];
    const vec4 tingParam    = parameters.vectors[0];
    
    const vec4 albedo = sampleTex(textureParam, 0, inUV);

    outColor = albedo * tingParam;
}