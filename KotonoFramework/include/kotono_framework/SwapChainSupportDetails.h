#pragma once
#include <vector>
#include <vulkan/vulkan_core.h>
struct KtSwapChainSupportDetails final
{
	VkSurfaceCapabilitiesKHR Capabilities;
	std::vector<VkSurfaceFormatKHR> Formats;
	std::vector<VkPresentModeKHR> PresentModes;
};