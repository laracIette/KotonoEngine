#include "common.glsl"

layout(location = 0) out vec3 outWorldPos;
layout(location = 1) out vec3 outNormal;
layout(location = 2) out vec2 outUV;
layout(location = 3) out vec4 outTangent;

// Helpers
#define UNPACK_PUSH_CONSTANTS                                                          \
    DrawData   drawData   = pc.frameContext.drawDatas.data[pc.drawIndex];               \
    Material   material   = pc.frameContext.materials.data[drawData.materialIndex];    \
    Transform  transform  = pc.frameContext.transforms.data[drawData.transformIndex];  \
    Parameters parameters = pc.frameContext.parameters.data[drawData.parametersIndex]; \
    Vertex     vertex     = pc.vertices.data[gl_VertexIndex];