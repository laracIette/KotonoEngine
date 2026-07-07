#pragma once
#include <kotono_common/types.h>
#include <vulkan/vulkan_core.h>
enum class ERenderBucket : u8
{
	Shadow,
	Opaque,
	Transparent,
	Interface,
};
struct UDrawCall
{
	// Push constants
	u32 index;
	VkDeviceAddress vertexBufferAdress;

	// Draw command
	VkPipeline pipeline;
	VkBuffer indexBuffer;
	u32 indexCount;
	u32 firstIndex;
	VkRect2D scissor;

	// Ordering
	ERenderBucket renderBucket;
	f32 sortKey; // Depth for 3D translucent, layer for 2D

	size poolIndex;
};