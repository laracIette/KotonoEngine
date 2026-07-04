#version 460
#include "../common_vert.glsl"

void main() {
    UNPACK_PUSH_CONSTANTS

    vec4 worldPos = transform.modelMatrix * vec4(vertex.position, 1.0);
    gl_Position = frame.viewProj * worldPos;

    outWorldPos = worldPos.xyz;
    outNormal   = mat3(transform.normalMatrix) * vertex.normal;
    outUV       = vertex.uv;
    outTangent  = vec4(mat3(transform.modelMatrix) * vertex.tangent.xyz, vertex.tangent.w);
}