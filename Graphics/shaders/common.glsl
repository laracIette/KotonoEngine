#extension GL_EXT_nonuniform_qualifier  : require
#extension GL_EXT_buffer_reference      : require
#extension GL_EXT_buffer_reference2     : require
#extension GL_EXT_scalar_block_layout   : require

struct DrawData {
    uint materialIndex;
    uint transformIndex;
    uint parametersIndex;
};

struct Material {
    uint albedoIndex;
	uint normalIndex;
	uint ormIndex;
	uint emissiveIndex;
	uint materialType;
	uint samplerIndex;
};

struct Transform {
    mat4 modelMatrix;
	mat4 normalMatrix;
};

struct Parameters {
    float scalars[16];
    vec4 vectors[16];
    uint textures[16]; // indices
};

struct Vertex {
    vec3 position;
    vec3 normal;
    vec2 uv;
    vec4 tangent;
};

struct Light {
    vec3 position;
    vec3 direction;
    vec3 color;
    float intensity;
    float range;
    float innerCone, outerCone;
    uint type;
    //uint shadowMapIndex; // index into bindless texture array, ~0u = no shadow
    //mat4 lightViewProj[4]; // cascades for directional, 1 entry otherwise
    //vec4 cascadeSplits; // directional only
};

// Global bindless resources
layout(set = 0, binding = 0)          uniform texture2D gTextures[];
layout(set = 0, binding = 1)          uniform sampler   gSamplers[];
layout(set = 0, binding = 2, rgba16f) uniform image2D   gImages[];

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
layout(buffer_reference, scalar) readonly buffer ParametersBuf {
    Parameters data[];
};
layout(buffer_reference, scalar) readonly buffer LightBuf {
    Light data[];
};
layout(buffer_reference, scalar) readonly buffer FrameContextBuf {
    DrawDataBuf   drawDatas;
    MaterialBuf   materials;
    TransformBuf  transforms;
    ParametersBuf parameters;
    LightBuf      lights;
    uint          lightCount;
};
layout(buffer_reference, scalar) readonly buffer VertexBuf {
    Vertex data[];
};

layout(push_constant, scalar) uniform PC {
    FrameContextBuf frameContext;
    VertexBuf       vertices;
    uint            drawIndex;
} pc;
