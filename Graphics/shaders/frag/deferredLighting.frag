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
    vec3  normal = texelFetch(gTextures[nonuniformEXT(frame.gBufferNormal)], px, 0).rgb;
    vec3  orm    = texelFetch(gTextures[nonuniformEXT(frame.gBufferORM)],    px, 0).rgb;
    float depth  = texelFetch(gTextures[nonuniformEXT(frame.gBufferDepth)],  px, 0).r;

    // Reversed-Z, far is 0.0
    if (depth <= 0.0)
    {
        outColor = vec4(0.0, 0.0, 0.0, 1.0);
        return;
    }

    vec3  N         = normalize(normal * 2.0 - 1.0); // Normal, assumes RGB-encoded normal; change if octahedral
    float ao        = orm.r;
    float roughness = clamp(orm.g, 0.045, 1.0);
    float metallic  = orm.b;

    vec3 worldPos = reconstructWorldPos(inUV, depth, frame.invViewProj);
    vec3 viewPos  = (frame.view * vec4(worldPos, 1.0)).xyz;
    vec3 camPos   = frame.viewPos.xyz;
    vec3 V        = normalize(camPos - worldPos); // view vector
   
    uint clusterIndex = computeClusterIndex(gl_FragCoord.xy, viewPos.z, tilePxSize, clusterScaleBias, clusterGridDim);
    LightGrid lightGrid = frame.clusterGrids.data[clusterIndex];

    vec3 F0 = mix(vec3(0.04), albedo, metallic); // Fresnel reflectance
    vec3 Lo = vec3(0.0);

    for (uint i = 0u; i < directionalLightCount; ++i)
    {
        DirectionalLight directionalLight = directionalLights.data[i];

        vec3 L = normalize(-directionalLight.direction); 
        vec3 H = normalize(V + L);
        
        // Linear intensity
        vec3 radiance = directionalLight.color * directionalLight.intensity;
        Lo += calculateLight(N, V, F0, L, H, albedo, roughness, metallic, radiance);
    }

    for (uint i = 0u; i < lightGrid.count; ++i)
    {
        // Point lights only
        PointLight pointLight = pointLights.data[frame.lightIndices.data[lightGrid.offset + i]];

        vec3  toLight = pointLight.position - worldPos;
        float dist    = length(toLight);
        float radius  = pointLight.range;
        if (dist > radius) continue;
        
        vec3 L = toLight / max(dist, 1e-4);
        vec3 H = normalize(V + L);
        
        float falloff = clamp(1.0 - pow(dist / radius, 4.0), 0.0, 1.0);
        float atten   = (falloff * falloff) / (dist * dist + 1.0);

        // Intensity falloff
        vec3 radiance = pointLight.color * pointLight.intensity * atten;
        Lo += calculateLight(N, V, F0, L, H, albedo, roughness, metallic, radiance);
    }

    vec3 ambient = vec3(0.03) * albedo * ao;
    outColor = vec4(ambient + Lo, 1.0);
}