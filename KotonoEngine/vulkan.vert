#version 460

struct ObjectData
{
    vec3 position;
    vec4 rotation;
    vec3 scale;
};

layout(set = 0, binding = 0) uniform CameraData 
{
    mat4 view;
    mat4 projection;
} cameraData;

layout(std140, set = 1, binding = 0) readonly buffer ObjectBuffer
{
	ObjectData objectDatas[];
} objectBuffer;

layout(location = 0) in vec3 inPosition;
// add normal
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;

mat4 translate(vec3 t);
mat4 quatToMat4(vec4 q);
mat4 scale(vec3 s);

void main() 
{
    ObjectData objectData = objectBuffer.objectDatas[gl_BaseInstance];
    vec3 objectPosition = objectData.position;
    vec4 objectRotation = objectData.rotation;
    vec3 objectScale = objectData.scale;
    mat4 model = translate(objectPosition) * quatToMat4(objectRotation) * scale(objectScale);

    gl_Position = cameraData.projection * cameraData.view * model * vec4(inPosition, 1.0);
    //gl_Position = vec4(0.5, 0.5, 0.0, 1.0);
    fragColor = inColor;
    fragTexCoord = inTexCoord;
}

mat4 translate(vec3 t)
{
    return mat4(
        1.0, 0.0, 0.0, t.x,
        0.0, 1.0, 0.0, t.y,
        0.0, 0.0, 1.0, t.z,
        0.0, 0.0, 0.0, 1.0
    );
}

mat4 quatToMat4(vec4 q)
{
    return mat4(
        1.0f - 2.0f * (q.y * q.y + q.z * q.z), 2.0f * (q.x * q.y - q.w * q.z),        2.0f * (q.x * q.z + q.w * q.y),        0.0f,
        2.0f * (q.x * q.y + q.w * q.z),        1.0f - 2.0f * (q.x * q.x + q.z * q.z), 2.0f * (q.y * q.z - q.w * q.x),        0.0f,
        2.0f * (q.x * q.z - q.w * q.y),        2.0f * (q.y * q.z + q.w * q.x),        1.0f - 2.0f * (q.x * q.x + q.y * q.y), 0.0f,
        0.0f,                                  0.0f,                                  0.0f,                                  1.0f
    );
}

mat4 scale(vec3 s) 
{
    return mat4(
        s.x, 0.0, 0.0, 0.0,
        0.0, s.y, 0.0, 0.0,
        0.0, 0.0, s.z, 0.0,
        0.0, 0.0, 0.0, 1.0
    );
}