#extension GL_EXT_nonuniform_qualifier  : require
#extension GL_EXT_buffer_reference      : require
#extension GL_EXT_buffer_reference2     : require
#extension GL_EXT_scalar_block_layout   : require


// Global bindless resources, not buffer_reference cause incompatible types
layout(set = 0, binding = 0)          uniform texture2D gTextures[];
layout(set = 0, binding = 1)          uniform sampler   gSamplers[];
layout(set = 0, binding = 2, rgba16f) uniform image2D   gImages[];


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
    vec4  vectors[16];
    uint  textures[16]; // indices
};

struct Vertex {
    vec3 position;
    vec3 normal;
    vec2 uv;
    vec4 tangent;
};

struct Light {
    vec3  position;
    vec3  direction;
    vec3  color;
    float intensity;
    float range;
    float innerCone;
    float outerCone;
    uint  type;
    //uint shadowMapIndex; // index into bindless texture array, ~0u = no shadow
    //mat4 lightViewProj[4]; // cascades for directional, 1 entry otherwise
    //vec4 cascadeSplits; // directional only
};

struct AABB {
    vec4 minPoint;
    vec4 maxPoint;
};

struct LightGrid {
    uint offset;
    uint count;
};

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
layout(buffer_reference, scalar) buffer ClusterAABBBuf {
    AABB data[];
};
layout(buffer_reference, scalar) buffer ClusterGridBuf {
    LightGrid data[];
};
layout(buffer_reference, scalar) buffer LightIndexBuf {
    uint data[];
};
layout(buffer_reference, scalar) buffer LightCounterBuf {
    uint data;
};


struct FrameContext {
    mat4            view;
    mat4            proj;
    mat4            invProj;
    mat4            viewProj;
    mat4            invViewProj;
    vec4            viewPos;

    float           time;

    DrawDataBuf     drawDatas;
    MaterialBuf     materials;
    TransformBuf    transforms;
    ParametersBuf   parameters;

    LightBuf        lights;
    uint            lightCount;

    ClusterAABBBuf  clusterAABBs;
    ClusterGridBuf  clusterGrids;
    LightIndexBuf   lightIndices;
    LightCounterBuf lightCounter;
};

layout(buffer_reference, scalar) readonly buffer FrameContextBuf {
    FrameContext data;
};
layout(buffer_reference, scalar) readonly buffer VertexBuf {
    Vertex data[];
};

layout(push_constant, scalar) uniform PC {
    FrameContextBuf frameContext;
    VertexBuf       vertices;
    uint            drawIndex;
} pc;
