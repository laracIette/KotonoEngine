#version 460

struct ObjectData
{
    mat4 model;
    vec4 color;
};

layout(set = 0, binding = 0) uniform CameraData 
{
    mat4 view;
    mat4 projection;
} cameraData;

layout(set = 1, binding = 0, std430) readonly buffer ObjectBuffer
{
	ObjectData objectDatas[];
} objectBuffer;

layout(location = 0) in vec3 inPosition;
// add normal
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) out vec4 color;

void main() 
{
    gl_Position = cameraData.projection * cameraData.view * objectBuffer.objectDatas[gl_InstanceIndex].model * vec4(inPosition, 1.0);
    fragColor = inColor;
    fragTexCoord = inTexCoord;
    color = objectBuffer.objectDatas[gl_InstanceIndex].color;
}