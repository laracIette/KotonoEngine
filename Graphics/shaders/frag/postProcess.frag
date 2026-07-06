#version 460
#include "../common_frag.glsl"

void main() 
{
	UNPACK_PUSH_CONSTANTS

    vec3 hdr = sampleTex(frame.postProcessTarget, frame.postProcessSampler, inUV).rgb;
    vec3 ldr = reinhard(hdr);

    outColor = vec4(ldr, 1.0);
}