#version 460
#include "../common.glsl"

layout(location = 0) out vec2 outUV;
layout(location = 1) out vec3 outNormal;

void main() {
    DrawData  d     = pc.drawData.data[pc.drawIndex];
    Transform trans = pc.transforms.data[d.transformIndex];
    Vertex    vert  = pc.vertices.data[gl_VertexIndex];

    gl_Position = frame.viewProj * trans.modelMatrix * vec4(vert.position, 1.0);

    outUV     = vert.uv;
    outNormal = mat3(trans.normalMatrix) * vert.normal;
}