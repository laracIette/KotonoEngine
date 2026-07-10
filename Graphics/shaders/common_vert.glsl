#include "common.glsl"

layout(location = 0) out vec3 outWorldPos;
layout(location = 1) out vec3 outNormal;
layout(location = 2) out vec2 outUV;
layout(location = 3) out vec4 outTangent;

// Helpers
#define UNPACK_PUSH_CONSTANTS                                                                   \
    FrameContext     frame            = pc.frameContext.data;                                   \
    float            time             = frame.time;                                             \
    DrawData         drawData         = frame.drawDatas.data[pc.drawIndex];                     \
    Material         material         = frame.materials.data[drawData.materialIndex];           \
    Transform        transform        = frame.transforms.data[drawData.transformIndex];         \
    Parameters       parameters       = frame.parameters.data[drawData.parametersIndex];        \
    DirectionalLight directionalLight = frame.directionalLights.data[pc.directionalLightIndex]; \
    Vertex           vertex           = pc.vertices.data[gl_VertexIndex];