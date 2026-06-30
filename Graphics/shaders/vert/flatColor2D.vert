#version 460
#include "../common.glsl"

void main() 
{
    UNPACK_PUSH_CONSTANTS_VERT

    gl_Position = transform.modelMatrix * vec4(vertex.position, 1.0);
}