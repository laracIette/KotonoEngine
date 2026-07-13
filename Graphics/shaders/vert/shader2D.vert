#version 460
#include "../common_vert.glsl"

layout(location = 0) out vec2 outUV;

void main() 
{
    UNPACK_PUSH_CONSTANTS

    gl_Position = transform.modelMatrix * vec4(vertex.position, 1.0);

    outUV = vertex.uv;
}