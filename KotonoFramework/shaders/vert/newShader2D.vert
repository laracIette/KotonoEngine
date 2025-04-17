#version 460
#extension GL_EXT_nonuniform_qualifier : enable
#extension GL_EXT_scalar_block_layout : enable

struct ObjectData
{
    mat4 model;
};

layout(set = 0, binding = 0, std430) readonly buffer ObjectBuffer
{
	ObjectData objectDatas[];
} objectBuffer;

layout(set = 0, binding = 1, std430) readonly buffer TextureIndexBuffer {
    uint texIndices[];
} textureIndexBuffer;

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) flat out uint textureIndex;

void main() 
{
    gl_Position = objectBuffer.objectDatas[gl_InstanceIndex].model * vec4(inPosition, 0.0, 1.0);
    fragColor = inColor;
    fragTexCoord = inTexCoord;
    textureIndex = textureIndexBuffer.texIndices[gl_InstanceIndex];
}