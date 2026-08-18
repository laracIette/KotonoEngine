#pragma once
#include <kotono_common/types.h>
#include <vulkan/vulkan_core.h>
struct UDrawDataBufferData
{
    u32 materialIndex;
    u32 transformIndex;
    u32 parametersIndex;
    VkDeviceAddress vertexBufferAddress;
};
