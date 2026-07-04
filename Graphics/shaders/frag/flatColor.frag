#version 460
#include "../common_frag.glsl"

void main() 
{
    UNPACK_PUSH_CONSTANTS

    const vec4 colorParam = parameters.vectors[0];

    outColor = colorParam;
}