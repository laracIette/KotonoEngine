#version 460
#include "../common_frag.glsl"

layout(location = 0) flat in int inInstanceIndex;

layout(location = 0) out vec4 outColor;

void main() 
{
    FrameContext        frame                 = pc.frameContext.data;
    float               time                  = frame.time;
    DrawData            drawData              = frame.drawDatas.data[pc.drawIndex + inInstanceIndex];
    Material            material              = frame.materials.data[drawData.materialIndex];
    Transform           transform             = frame.transforms.data[drawData.transformIndex];
    Parameters          parameters            = frame.parameters.data[drawData.parametersIndex];
    DirectionalLightBuf directionalLights     = frame.directionalLights;
    uint                directionalLightCount = frame.directionalLightCount;
    PointLightBuf       pointLights           = frame.pointLights;
    uint                pointLightCount       = frame.pointLightCount;

    const vec4 colorParam = parameters.vectors[0];

    outColor = colorParam;
}