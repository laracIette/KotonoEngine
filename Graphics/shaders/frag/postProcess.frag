#version 460
#include "../common_frag.glsl"

vec3 reinhard(vec3 hdrColor)
{
    return hdrColor / (hdrColor + vec3(1.0));
}

layout(location = 0) in vec2 inUV;

layout(location = 0) out vec4 outColor;

void main() 
{
	UNPACK_PUSH_CONSTANTS

    vec3 hdr = sampleTex(frame.colorTarget, frame.colorSampler, inUV).rgb;
    vec3 ldr = reinhard(hdr);

    outColor = vec4(ldr, 1.0);
}