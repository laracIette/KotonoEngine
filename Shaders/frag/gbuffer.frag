#version 460
#include "../common_frag.glsl"

layout(location = 0) in vec3 inWorldPos;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inUV;
layout(location = 3) in vec4 inTangent;

layout(location = 0) out vec4 outAlbedo;
layout(location = 1) out vec4 outNormal;
layout(location = 2) out vec4 outORM;

vec3 getNormalFromMap(Material mat, vec2 uv) 
{
    vec3 N = normalize(inNormal);
    vec3 T = normalize(inTangent.xyz - N * dot(inTangent.xyz, N));
    vec3 B = cross(N, T) * inTangent.w;
    mat3 TBN = mat3(T, B, N);

    vec3 tangentNormal = sampleTex(mat.normalIndex, mat.samplerIndex, uv).xyz * 2.0 - 1.0;
    return normalize(TBN * tangentNormal);
}

void main() {
	UNPACK_PUSH_CONSTANTS

	vec3 normal      = getNormalFromMap(material, inUV);
	vec3 worldNormal = normalize(mat3(transform.normalMatrix) * normal);

	outAlbedo = sampleTex(material.albedoIndex, material.samplerIndex, inUV);
	outNormal = vec4(worldNormal, 0.0);
	outORM    = sampleTex(material.ormIndex, material.samplerIndex, inUV);
}