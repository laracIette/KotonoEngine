#pragma once
#include <kotono_common/types.h>
#include <vulkan/vulkan_core.h>
struct UDrawCall
{
	// Push constants
	u32 index;

	// Draw command
	VkPipeline pipeline;
	u32 indexCount;
	u32 firstIndex;
	VkRect2D scissor;

	// Ordering
	f32 sortKey; // Depth for 3D translucent, layer for 2D

	size poolIndex;
};