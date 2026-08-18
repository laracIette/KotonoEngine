#pragma once
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_uint2.hpp>
#include <kotono_common/types.h>
#include <vulkan/vulkan_core.h>
struct UFrameContextBufferData
{
    glm::mat4 view;
    glm::mat4 proj;
    glm::mat4 invProj;
    glm::mat4 viewProj;
    glm::mat4 invViewProj;
    glm::vec3 viewPos;

    glm::uvec2 windowSize;

    f32 time;

    VkDeviceAddress drawDataBufferAddress;
    VkDeviceAddress materialBufferAddress;
    VkDeviceAddress transformBufferAddress;
    VkDeviceAddress parametersBufferAddress;

    VkDeviceAddress directionalLightBufferAddress;
    VkDeviceAddress pointLightBufferAddress;
    u32             directionalLightCount;
    u32             pointLightCount;

    VkDeviceAddress clusterAABBBufferAddress;
    VkDeviceAddress clusterGridBufferAddress;
    VkDeviceAddress lightIndexBufferAddress;
    VkDeviceAddress lightCounterBufferAddress;

    u32 gBufferDepth;
    u32 gBufferAlbedo;
    u32 gBufferNormal;
    u32 gBufferORM;
    u32 gBufferSampler;

    u32 colorTarget;
    u32 colorSampler;
};

struct UFrameContextSceneView
{ 
    glm::mat4 view;
    glm::mat4 proj;
    glm::vec3 viewPos;
    glm::uvec2 extent;
    f32 time;
    f32 fov;
    f32 aspectRatio;
    f32 depthNear;
};

struct UFrameContextAddresses
{
    VkDeviceAddress drawDataBufferAddress;
    VkDeviceAddress materialBufferAddress;
    VkDeviceAddress transformBufferAddress;
    VkDeviceAddress parametersBufferAddress;

    VkDeviceAddress directionalLightBufferAddress;
    VkDeviceAddress pointLightBufferAddress;

    VkDeviceAddress clusterAABBBufferAddress;
    VkDeviceAddress clusterGridBufferAddress;
    VkDeviceAddress lightIndexBufferAddress;
    VkDeviceAddress lightCounterBufferAddress;
};

struct UFrameContextTargets
{
    u32 gBufferDepth;
    u32 gBufferAlbedo;
    u32 gBufferNormal;
    u32 gBufferORM;
    u32 colorTarget;
};
