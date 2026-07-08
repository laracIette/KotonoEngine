#include "common.glsl"
#extension GL_EXT_samplerless_texture_functions : require

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
#define UNPACK_PUSH_CONSTANTS                                                                    \
    FrameContext        frame                 = pc.frameContext.data;                            \
    DrawData            drawData              = frame.drawDatas.data[pc.drawIndex];              \
    Material            material              = frame.materials.data[drawData.materialIndex];    \
    Transform           transform             = frame.transforms.data[drawData.transformIndex];  \
    Parameters          parameters            = frame.parameters.data[drawData.parametersIndex]; \
    DirectionalLightBuf directionalLights     = frame.directionalLights;                         \
    uint                directionalLightCount = frame.directionalLightCount;                     \
    PointLightBuf       pointLights           = frame.pointLights;                               \
    uint                pointLightCount       = frame.pointLightCount;

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





vec3 getNormalFromMap(Material mat, vec2 uv) {
    vec3 N = normalize(inNormal);
    vec3 T = normalize(inTangent.xyz - N * dot(inTangent.xyz, N));
    vec3 B = cross(N, T) * inTangent.w;
    mat3 TBN = mat3(T, B, N);

    vec3 tangentNormal = sampleTex(mat.normalIndex, mat.samplerIndex, uv).xyz * 2.0 - 1.0;
    return normalize(TBN * tangentNormal);
}

void unpackORM(Material mat, vec2 uv, out float occlusion, out float roughness, out float metallic) {
    vec4 orm = sampleTex(mat.ormIndex, mat.samplerIndex, uv);
    occlusion = orm.r;
    roughness = orm.g;
    metallic  = orm.b;
}



// Reconstruct World Position from Depth Buffer
vec3 reconstructWorldPos(vec2 uv, float depth, mat4 invViewProj) 
{
    vec4 clipSpace = vec4(uv * 2.0 - 1.0, depth, 1.0);
    vec4 worldSpace = invViewProj * clipSpace;
    return worldSpace.xyz / worldSpace.w;
}

// Doom-2016-style logarithmic Z slicing.
// CPU side, per frame:
//   scale = numZSlices / log2(far / near)
//   bias  = -numZSlices * log2(near) / log2(far / near)
uint computeClusterIndex(vec2 fragCoordPx, float viewSpaceZ, float tilePxSize, vec2 clusterScaleBias, uvec3 clusterGridDim)
{
    uvec3 c;
    c.x = uint(fragCoordPx.x / tilePxSize);
    c.y = uint(fragCoordPx.y / tilePxSize);

    float linearZ = -viewSpaceZ; // view space looks down -Z
    c.z = uint(clamp(
        log2(max(linearZ, 1e-4)) * clusterScaleBias.x + clusterScaleBias.y,
        0.0, float(clusterGridDim.z - 1)));

    c = min(c, clusterGridDim - uvec3(1));

    return c.x
         + clusterGridDim.x * c.y
         + clusterGridDim.x * clusterGridDim.y * c.z;
}

// GGX / Trowbridge-Reitz normal distribution
float distributionGGX(vec3 N, vec3 H, float roughness) 
{
    float a  = roughness * roughness;
    float a2 = a * a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;
    float d  = (NdotH2 * (a2 - 1.0) + 1.0);
    return a2 / (PI * d * d + 1e-5);
}

// Schlick-GGX geometry term, combined via Smith's method
float geometrySchlickGGX(float NdotX, float roughness) 
{
    float r = roughness + 1.0;
    float k = (r * r) / 8.0; // direct-light remapping
    return NdotX / (NdotX * (1.0 - k) + k);
}

float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness) 
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    return geometrySchlickGGX(NdotV, roughness) * geometrySchlickGGX(NdotL, roughness);
}

// Schlick Fresnel approximation
vec3 fresnelSchlick(float cosTheta, vec3 F0) 
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

vec3 calculateLight(vec3 N, vec3 V, vec3 F0, vec3 L, vec3 H, vec3 albedo, float roughness, float metallic, vec3 radiance)
{
    float NDF = distributionGGX(N, H, roughness);
    float G   = geometrySmith(N, V, L, roughness);
    vec3  F   = fresnelSchlick(max(dot(H, V), 0.0), F0);
        
    vec3  numerator = NDF * G * F;
    float denom     = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 1e-4;
    vec3  specular  = numerator / denom;
        
    vec3  kD    = (vec3(1.0) - F) * (1.0 - metallic);
    float NdotL = max(dot(N, L), 0.0);
        
    return (kD * albedo / PI + specular) * radiance * NdotL;
}



// Post-process
vec3 reinhard(vec3 hdrColor)
{
    return hdrColor / (hdrColor + vec3(1.0));
}
