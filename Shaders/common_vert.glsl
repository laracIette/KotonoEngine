#include "common.glsl"

// Helpers
#define UNPACK_PUSH_CONSTANTS                                                                   \
    FrameContext     frame            = pc.frameContext.data;                                   \
    float            time             = frame.time;                                             \
    DrawData         drawData         = frame.drawDatas.data[pc.drawIndex];                     \
    Material         material         = frame.materials.data[drawData.materialIndex];           \
    Transform        transform        = frame.transforms.data[drawData.transformIndex];         \
    Parameters       parameters       = frame.parameters.data[drawData.parametersIndex];        \
    DirectionalLight directionalLight = frame.directionalLights.data[pc.directionalLightIndex]; \
    Vertex           vertex           = drawData.vertices.data[gl_VertexIndex];