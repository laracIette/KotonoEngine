#version 460
#include "../common_frag.glsl"

void main() 
{
    UNPACK_PUSH_CONSTANTS

    const uint textureParam = parameters.textures[0];
    
    const vec4 albedo = sampleTex(textureParam, 0, inUV);

    outColor = albedo;
}