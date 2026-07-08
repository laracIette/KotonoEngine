#version 460
#include "../common_frag.glsl"

void main() {
    UNPACK_PUSH_CONSTANTS

    outColor = vec4(1.0, 1.0, 1.0, 1.0);
}