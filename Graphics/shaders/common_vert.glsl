#include "common.glsl"

layout(location = 0) out vec3 outWorldPos;
layout(location = 1) out vec3 outNormal;
layout(location = 2) out vec2 outUV;
layout(location = 3) out vec4 outTangent;

// Helpers
#define UNPACK_PUSH_CONSTANTS                                        \
    DrawData   drawData   = pc.drawData.data[pc.drawIndex];               \
    Material   material   = pc.materials.data[drawData.materialIndex];    \
    Transform  transform  = pc.transforms.data[drawData.transformIndex];  \
    Parameters parameters = pc.parameters.data[drawData.parametersIndex]; \
    Vertex     vertex     = pc.vertices.data[gl_VertexIndex];