#pragma once

#include "Containers/List.h"

#include <vulkan/vulkan_core.h>

struct USwapchainSupportDetails final
{
	VkSurfaceCapabilitiesKHR capabilities;
	UList<VkSurfaceFormatKHR> formats;
	UList<VkPresentModeKHR> presentModes;
};