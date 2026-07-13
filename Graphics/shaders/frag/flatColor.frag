#version 460
#include "../common_frag.glsl"

layout(location = 0) out vec4 outColor;

void main() 
{
    UNPACK_PUSH_CONSTANTS

    const vec4 colorParam = parameters.vectors[0];

    outColor = colorParam;
}