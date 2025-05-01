#version 460

struct ObjectData
{
    mat4 model;
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

void main() 
{
    gl_Position = cameraData.projection * cameraData.view * objectBuffer.objectDatas[gl_InstanceIndex].model * vec4(inPosition, 1.0);
}