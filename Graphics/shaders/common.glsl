#extension GL_EXT_nonuniform_qualifier  : require
#extension GL_EXT_buffer_reference      : require
#extension GL_EXT_buffer_reference2     : require
#extension GL_EXT_scalar_block_layout   : require

struct DrawData {
    uint materialIndex;
    uint transformIndex;
    uint meshletOffset;
};

struct Material {
    uint albedoIndex;
	uint normalIndex;
	uint roughnessIndex;
	uint emissiveIndex;
	uint materialType;
	uint samplerIndex;
};

struct Transform {
    mat4 modelMatrix;
	mat4 normalMatrix;
};

struct Vertex {
    vec3 position;
    vec3 normal;
    vec2 uv;
    vec4 tangent;
};

// Global bindless resources
layout(set = 0, binding = 0)            uniform texture2D gTextures[];
layout(set = 0, binding = 1)            uniform sampler   gSamplers[];
layout(set = 0, binding = 2, rgba16f)   uniform image2D   gImages[];

// Uniform 
layout(set = 1, binding = 0) uniform FrameUBO {
    mat4  view, proj, viewProj;
    vec4  viewPos;
    float time;
} frame;

// BDA struct definitions
layout(buffer_reference, scalar) readonly buffer DrawDataBuf {
    DrawData data[];
};
layout(buffer_reference, scalar) readonly buffer MaterialBuf {
    Material data[];
};
layout(buffer_reference, scalar) readonly buffer TransformBuf {
    Transform data[];
};
layout(buffer_reference, scalar) readonly buffer VertexBuf {
    Vertex data[];
};

layout(push_constant, scalar) uniform PC {
    DrawDataBuf  drawData;
    MaterialBuf  materials;
    TransformBuf transforms;
    VertexBuf vertices;
    uint drawIndex;
    uint flags;
} pc;

// Convenience helpers
vec4 sampleTex(uint texIdx, uint sampIdx, vec2 uv) {
    return texture(
        sampler2D(gTextures[nonuniformEXT(texIdx)],
                  gSamplers[nonuniformEXT(sampIdx)]),
        uv);
}

vec4 sampleTexLod(uint texIdx, uint sampIdx, vec2 uv, float lod) {
    return textureLod(
        sampler2D(gTextures[nonuniformEXT(texIdx)],
                  gSamplers[nonuniformEXT(sampIdx)]),
        uv, lod);
}