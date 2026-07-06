#version 460
#include "../common_vert.glsl"

void main() {
    UNPACK_PUSH_CONSTANTS

    // Generates UV coordinates: 
    // Index 0: (0.0, 0.0)
    // Index 1: (2.0, 0.0)
    // Index 2: (0.0, 2.0)
    outUV = vec2((gl_VertexIndex << 1) & 2, gl_VertexIndex & 2);
    
    // Convert UV to Clip Space Positions: [-1.0, 1.0]
    // Index 0: (-1.0, -1.0)
    // Index 1: ( 3.0, -1.0)
    // Index 2: (-1.0,  3.0)
    gl_Position = vec4(outUV * 2.0 - 1.0, 0.0, 1.0);
}