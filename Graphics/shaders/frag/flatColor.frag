#version 460
#include "../common.glsl"

layout(location = 0) out vec4 outColor;

void main() 
{
    UNPACK_PUSH_CONSTANTS_FRAG

    const vec4 colorParam = parameters.vectors[0];

    outColor = colorParam;
}