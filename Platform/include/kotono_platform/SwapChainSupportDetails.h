#pragma once
#include <vector>
#include <vulkan/vulkan_core.h>
struct KtSwapChainSupportDetails final
{
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};