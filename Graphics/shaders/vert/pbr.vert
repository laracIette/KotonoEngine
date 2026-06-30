#version 460
#include "../common.glsl"

layout(location = 0) out vec2 outUV;
layout(location = 1) out vec3 outNormal;

void main() {
    UNPACK_PUSH_CONSTANTS_VERT

    gl_Position = frame.viewProj * transform.modelMatrix * vec4(vertex.position, 1.0);

    outUV     = vertex.uv;
    outNormal = mat3(transform.normalMatrix) * vertex.normal;
}