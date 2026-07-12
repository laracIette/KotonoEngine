#extension GL_EXT_nonuniform_qualifier        : require
#extension GL_EXT_buffer_reference            : require
#extension GL_EXT_buffer_reference2           : require
#extension GL_EXT_scalar_block_layout         : require
#extension GL_EXT_shader_image_load_formatted : require
#extension GL_EXT_multiview                   : require

// Define all buffers' access mode,
// forces to specify a valid mode or leave readonly
#ifndef ACCESS_CLUSTER_AABB_BUF
#define ACCESS_CLUSTER_AABB_BUF readonly
#endif
#ifndef ACCESS_CLUSTER_GRID_BUF
#define ACCESS_CLUSTER_GRID_BUF readonly
#endif
#ifndef ACCESS_LIGHT_INDEX_BUF
#define ACCESS_LIGHT_INDEX_BUF readonly
#endif
#ifndef ACCESS_LIGHT_COUNTER_BUF
#define ACCESS_LIGHT_COUNTER_BUF readonly
#endif

// Global bindless resources, not buffer_reference cause incompatible types
layout(set = 0, binding = 0) uniform texture2D      gTextures[];
layout(set = 0, binding = 1) uniform texture2DArray gTextureArrays[];
layout(set = 0, binding = 2) uniform sampler        gSamplers[];
layout(set = 0, binding = 3) uniform samplerShadow  gShadowSamplers[];
layout(set = 0, binding = 4) uniform image2D        gImages[];


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

const uint NUM_DIRECTIONAL_CASCADES = 4;

struct DirectionalLight {
    vec3  direction;
	vec3  color;
	float intensity;
    uint  castShadow;
    uint  shadowMap;
	uint  shadowSampler;
	mat4  lightViewProjs[NUM_DIRECTIONAL_CASCADES];
    float cascadeSplits[NUM_DIRECTIONAL_CASCADES];
};

struct PointLight {
    vec3  position;
    float range;
    vec3  color;
    float intensity;
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
layout(buffer_reference, scalar) readonly buffer DirectionalLightBuf {
    DirectionalLight data[];
};
layout(buffer_reference, scalar) readonly buffer PointLightBuf {
    PointLight data[];
};
layout(buffer_reference, scalar) ACCESS_CLUSTER_AABB_BUF buffer ClusterAABBBuf {
    AABB data[];
};
layout(buffer_reference, scalar) ACCESS_CLUSTER_GRID_BUF buffer ClusterGridBuf {
    LightGrid data[];
};
layout(buffer_reference, scalar) ACCESS_LIGHT_INDEX_BUF buffer LightIndexBuf {
    uint data[];
};
layout(buffer_reference, scalar) ACCESS_LIGHT_COUNTER_BUF buffer LightCounterBuf {
    uint data;
};

struct FrameContext {
    mat4 view;
    mat4 proj;
    mat4 invProj;
    mat4 viewProj;
    mat4 invViewProj;
    vec3 viewPos;
          
    float time;
                     
    DrawDataBuf   drawDatas;
    MaterialBuf   materials;
    TransformBuf  transforms;
    ParametersBuf parameters;

    DirectionalLightBuf directionalLights;
    PointLightBuf       pointLights;
    uint                directionalLightCount;
    uint                pointLightCount;
                     
    ClusterAABBBuf  clusterAABBs;
    ClusterGridBuf  clusterGrids;
    LightIndexBuf   lightIndices;
    LightCounterBuf lightCounter;
                     
    uint gBufferAlbedo;
    uint gBufferNormal;
    uint gBufferORM;
    uint gBufferDepth;
    uint gBufferSampler;
         
    uint postProcessTarget;
    uint postProcessSampler;
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
    uint            directionalLightIndex;
} pc;

const float PI = 3.14159265;

const float Z_NEAR = 0.1;
const float Z_FAR = 1000.0;
