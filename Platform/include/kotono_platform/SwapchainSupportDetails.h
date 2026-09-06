#pragma once
#include <vector>
#include <vulkan/vulkan_core.h>
struct USwapchainSupportDetails final
{
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};