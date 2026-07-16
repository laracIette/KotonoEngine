#pragma once
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
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

    u32 gBufferAlbedo;
    u32 gBufferNormal;
    u32 gBufferORM;
    u32 gBufferDepth;
    u32 gBufferSampler;

    u32 postProcessTarget;
    u32 postProcessSampler;
};
