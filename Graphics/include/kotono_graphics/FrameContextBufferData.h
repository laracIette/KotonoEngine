#pragma once
#include <kotono_common/types.h>
#include <vulkan/vulkan_core.h>
struct UFrameContextBufferData
{
    VkDeviceAddress drawDataBufferAddress;
    VkDeviceAddress materialBufferAddress;
    VkDeviceAddress transformBufferAddress;
    VkDeviceAddress parametersBufferAddress;
    VkDeviceAddress lightBufferAdress;
    u32 lightCount;
};
