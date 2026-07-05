#pragma once
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <kotono_common/types.h>
#include <vulkan/vulkan_core.h>
struct UFrameContextBufferData
{
    glm::mat4 view;
    glm::mat4 proj;
    glm::mat4 viewProj;
    glm::vec4 viewPos;

    f32 time;

    VkDeviceAddress drawDataBufferAddress;
    VkDeviceAddress materialBufferAddress;
    VkDeviceAddress transformBufferAddress;
    VkDeviceAddress parametersBufferAddress;

    VkDeviceAddress lightBufferAddress;
    u32 lightCount;

    VkDeviceAddress clusterAABBBufferAddress;
    VkDeviceAddress clusterGridBufferAddress;
    VkDeviceAddress lightIndexBufferAddress;
};
