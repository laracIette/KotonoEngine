#version 460
#include "../common_frag.glsl"

layout(location = 0) flat in int inInstanceIndex;
layout(location = 1) in vec2 inUV;

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

    const uint textureParam = parameters.textures[0];
    const vec4 tintParam    = parameters.vectors[0];
    
    const vec4 albedo = sampleTex(textureParam, 0, inUV);

    outColor = albedo * tintParam;
}