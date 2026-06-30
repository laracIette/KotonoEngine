#version 460
#include "../common.glsl"

layout(location = 0) out vec2 outUV;

void main() 
{
    UNPACK_PUSH_CONSTANTS_VERT

    gl_Position = transform.modelMatrix * vec4(vertex.position, 1.0);

    outUV = vertex.uv;
}