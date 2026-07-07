#version 460
#include "../common_frag.glsl"

void main() {
    UNPACK_PUSH_CONSTANTS
    
    ivec2 px = ivec2(gl_FragCoord.xy);

    uvec3 clusterGridDim = uvec3(16, 9, 24);
    float tilePxSize = 32;

    float numZSlices = clusterGridDim.z;
    float scale = float(numZSlices) / log2(Z_FAR / Z_NEAR);
    float bias  = -float(numZSlices) * log2(Z_NEAR) / log2(Z_FAR / Z_NEAR);

    vec2 clusterScaleBias = vec2(scale, bias);

    // texelFetch, not texture(): the G-buffer is the same resolution as this
    // pass's target, so we want an exact texel, not a filtered/interpolated one.
    vec3  albedo = texelFetch(gTextures[nonuniformEXT(frame.gBufferAlbedo)], px, 0).rgb;
    vec3  encN   = texelFetch(gTextures[nonuniformEXT(frame.gBufferNormal)], px, 0).rgb;
    vec3  orm    = texelFetch(gTextures[nonuniformEXT(frame.gBufferORM)],    px, 0).rgb;
    float depth  = texelFetch(gTextures[nonuniformEXT(frame.gBufferDepth)],  px, 0).r;

    // Reversed-Z, far is 0.0
    if (depth <= 0.0)
    {
        outColor = vec4(0.0, 0.0, 0.0, 1.0);
        return;
    }

    vec3  N         = normalize(encN * 2.0 - 1.0); // assumes RGB-encoded normal; change if octahedral
    float ao        = orm.r;
    float roughness = clamp(orm.g, 0.045, 1.0);
    float metallic  = orm.b;

    vec3 worldPos = reconstructWorldPos(inUV, depth, frame.invViewProj);
    vec3 viewPos  = (frame.view * vec4(worldPos, 1.0)).xyz;
    vec3 camPos   = frame.viewPos.xyz;
    vec3 V        = normalize(camPos - worldPos);

    uint clusterIndex = computeClusterIndex(gl_FragCoord.xy, viewPos.z, tilePxSize, clusterScaleBias, clusterGridDim);
    LightGrid lightGrid = frame.clusterGrids.data[clusterIndex];

    vec3 F0 = mix(vec3(0.04), albedo, metallic);
    vec3 Lo = vec3(0.0);

    for (uint i = 0u; i < lightGrid.count; ++i)
    {
        Light light = lights.data[frame.lightIndices.data[lightGrid.offset + i]];

        if (light.type != LIGHT_TYPE_POINT) continue;

        vec3  toLight = light.position - worldPos;
        float dist    = length(toLight);
        float radius  = light.range;
        if (dist > radius) continue;

        vec3 L = toLight / max(dist, 1e-4);
        vec3 H = normalize(V + L);

        float falloff  = clamp(1.0 - pow(dist / radius, 4.0), 0.0, 1.0);
        float atten    = (falloff * falloff) / (dist * dist + 1.0);
        vec3  radiance = light.color * light.intensity * atten;

        float NDF = distributionGGX(N, H, roughness);
        float G   = geometrySmith(N, V, L, roughness);
        vec3  F   = fresnelSchlick(max(dot(H, V), 0.0), F0);

        vec3  numerator = NDF * G * F;
        float denom     = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 1e-4;
        vec3  specular  = numerator / denom;

        vec3  kD    = (vec3(1.0) - F) * (1.0 - metallic);
        float NdotL = max(dot(N, L), 0.0);

        Lo += (kD * albedo / PI + specular) * radiance * NdotL;
    }

    vec3 ambient = vec3(0.03) * albedo * ao;
    outColor = vec4(ambient + Lo, 1.0);
}