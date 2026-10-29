#version 460
#include "../common_frag.glsl"

layout(location = 0) flat in int inInstanceIndex;
layout(location = 1) in vec2 inUV;

layout(location = 0) out vec4 outColor;

float sdRoundedBox(vec2 p, vec2 b, vec4 r) {
    // Select the appropriate corner radius based on the quadrant
    float radius = (p.x < 0.0) ?
    ((p.y > 0.0) ? r.x : r.w) : // Top-Left (x) or Bottom-Left (w)
    ((p.y > 0.0) ? r.y : r.z);  // Top-Right (y) or Bottom-Right (z)

    vec2 q = abs(p) - b + vec2(radius);
    return length(max(q, 0.0)) + min(max(q.x, q.y), 0.0) - radius;
}

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

    const uint textureParam  = parameters.textures[0];
    const vec4 tintParam     = parameters.vectors[0];
    const vec4 cornersParam  = parameters.vectors[1];
    const vec2 positionParam = parameters.vectors[2].xy;
    const vec2 sizeParam     = parameters.vectors[2].zw;

    vec2 pixelCoord = inUV * sizeParam;
    vec2 halfSize = sizeParam * 0.5;

    vec2 p = pixelCoord - halfSize;
    float d = sdRoundedBox(p, halfSize, cornersParam);
    float alpha = 1.0 - smoothstep(0.0, fwidth(d), d);

    const vec4 albedo = sampleTex(textureParam, 0, inUV);
    
    outColor = albedo * tintParam;
    outColor.a *= alpha;
}
