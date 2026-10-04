#version 460
#include "../common_vert.glsl"

layout(location = 0) flat out int outInstanceIndex;
layout(location = 1) out vec2 outUV;

void main() 
{
    FrameContext     frame            = pc.frameContext.data;
    float            time             = frame.time;
    DrawData         drawData         = frame.drawDatas.data[pc.drawIndex + gl_InstanceIndex];
    Material         material         = frame.materials.data[drawData.materialIndex];
    Transform        transform        = frame.transforms.data[drawData.transformIndex];
    Parameters       parameters       = frame.parameters.data[drawData.parametersIndex];
    DirectionalLight directionalLight = frame.directionalLights.data[pc.directionalLightIndex];
    Vertex           vertex           = drawData.vertices.data[gl_VertexIndex];

    gl_Position = transform.modelMatrix * vec4(vertex.position, 1.0);

    outInstanceIndex = gl_InstanceIndex;
    outUV = vertex.uv;
}