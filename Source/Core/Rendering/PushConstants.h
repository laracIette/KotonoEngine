#pragma once
#include <kotono_common/types.h>
#include <vulkan/vulkan_core.h>
struct UPushConstants
{
	VkDeviceAddress frameContextBufferAddress;
	u32 drawIndex;
	u32 directionalIndex;
};