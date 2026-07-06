#version 460
#include "../common_vert.glsl"

void main() {
	UNPACK_PUSH_CONSTANTS

    // Transform to world space, then to clip space
    vec4 worldPos = transform.modelMatrix * vec4(vertex.position, 1.0);
    gl_Position = frame.viewProj * worldPos;
}