#version 460
#include "../common_frag.glsl"

void main() {
    UNPACK_PUSH_CONSTANTS

    vec4 albedo   = sampleTex(material.albedoIndex,   material.samplerIndex, inUV);
    vec3 emissive = sampleTex(material.emissiveIndex, material.samplerIndex, inUV).rgb;
    float occlusion, roughness, metallic;
    unpackORM(material, inUV, occlusion, roughness, metallic);
    
    vec3 N = getNormalFromMap(material, inUV);
    vec3 V = normalize(frame.viewPos.xyz - inWorldPos);

    // Dielectrics get a flat 0.04 F0, metals use albedo as F0.
    vec3 F0 = mix(vec3(0.04), albedo.rgb, metallic);

    vec3 Lo = vec3(0.0);

    for (uint i = 0u; i < lightCount; ++i) {
        Light light = lights.data[i];

        vec3  L;
        float attenuation = 1.0;
        if (light.type == LIGHT_TYPE_DIRECTIONAL) 
        {
            L = normalize(-light.direction);
        } 
        else if (light.type == LIGHT_TYPE_POINT) 
        {
            vec3 toLight = light.position - inWorldPos;
            float dist = length(toLight);
            L = toLight / max(dist, 1e-4);
            float falloff = pow(clamp(1.0 - pow(dist / light.range, 4.0), 0.0, 1.0), 2.0);
            attenuation = falloff / max(dist * dist, 1e-4);
        }

        vec3 H = normalize(V + L);
        vec3 radiance = light.color * light.intensity * attenuation;

        float NDF = distributionGGX(N, H, roughness);
        float G   = geometrySmith(N, V, L, roughness);
        vec3  F   = fresnelSchlick(max(dot(H, V), 0.0), F0);

        vec3  numerator = NDF * G * F;
        float denom     = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 1e-4;
        vec3  specular  = numerator / denom;

        vec3 kS = F;
        vec3 kD = (vec3(1.0) - kS) * (1.0 - metallic); // metals have no diffuse term

        float NdotL = max(dot(N, L), 0.0);
        Lo += (kD * albedo.rgb / PI + specular) * radiance * NdotL; 
    }

    // Flat placeholder ambient — swap for IBL (irradiance + prefiltered specular
    // + BRDF LUT) once the shadowless direct-lighting pass is validated.
    vec3 ambient = vec3(0.03) * albedo.rgb * occlusion;

    vec3 color = ambient + Lo + emissive;

    // Reinhard tonemap: compress HDR values between 0 and 1
    color = color / (color + vec3(1.0));
    outColor = vec4(color, albedo.a);
}