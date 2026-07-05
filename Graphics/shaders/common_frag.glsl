#include "common.glsl"

layout(location = 0) in vec3 inWorldPos;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inUV;
layout(location = 3) in vec4 inTangent;

#ifdef OUTPUT_GBUFFER
layout(location = 0) out vec4 outAlbedo;
layout(location = 1) out vec4 outNormal;
layout(location = 2) out vec4 outORM;
#else
layout(location = 0) out vec4 outColor;
#endif

// Helpers
#define UNPACK_PUSH_CONSTANTS                                             \
    DrawData   drawData   = pc.drawData.data[pc.drawIndex];               \
    Material   material   = pc.materials.data[drawData.materialIndex];    \
    Transform  transform  = pc.transforms.data[drawData.transformIndex];  \
    Parameters parameters = pc.parameters.data[drawData.parametersIndex]; \
    LightBuf   lights     = pc.lights;                                    \
    uint       lightCount = pc.lightCount;
    
const uint LIGHT_TYPE_DIRECTIONAL = 0u;
const uint LIGHT_TYPE_POINT = 1u;
const uint LIGHT_TYPE_SPOT = 2u;

const float PI = 3.14159265;

vec4 sampleTex(uint texIdx, uint sampIdx, vec2 uv) {
    return texture(
        sampler2D(gTextures[nonuniformEXT(texIdx)], gSamplers[nonuniformEXT(sampIdx)]),
        uv
    );
}

vec4 sampleTexLod(uint texIdx, uint sampIdx, vec2 uv, float lod) {
    return textureLod(
        sampler2D(gTextures[nonuniformEXT(texIdx)], gSamplers[nonuniformEXT(sampIdx)]), 
        uv, lod
    );
}

// GGX / Trowbridge-Reitz normal distribution
float distributionGGX(vec3 N, vec3 H, float roughness) {
    float a  = roughness * roughness;
    float a2 = a * a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;
    float denom  = (NdotH2 * (a2 - 1.0) + 1.0);
    return a2 / max(PI * denom * denom, 1e-7);
}

// Schlick-GGX geometry term, combined via Smith's method
float geometrySchlickGGX(float NdotV, float roughness) {
    float r = roughness + 1.0;
    float k = (r * r) / 8.0; // direct-light remapping
    return NdotV / max(NdotV * (1.0 - k) + k, 1e-7);
}

float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    return geometrySchlickGGX(NdotV, roughness) * geometrySchlickGGX(NdotL, roughness);
}

// Schlick Fresnel approximation
vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

vec3 getNormalFromMap(Material mat, vec2 uv) {
    vec3 tangentNormal = sampleTex(mat.normalIndex, mat.samplerIndex, uv).xyz * 2.0 - 1.0;

    vec3 N = normalize(inNormal);
    vec3 T = normalize(inTangent.xyz - N * dot(inTangent.xyz, N));
    vec3 B = cross(N, T) * inTangent.w;
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

void unpackORM(Material mat, vec2 uv, out float occlusion, out float roughness, out float metallic) {
    vec4 orm = sampleTex(mat.ormIndex, mat.samplerIndex, uv);
    occlusion = orm.r;
    roughness = orm.g;
    metallic  = orm.b;
}