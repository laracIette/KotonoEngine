#pragma once
#include <vulkan/vulkan_core.h>
struct KtViewport
{
    // perchance change to float
    VkOffset2D Offset;
    VkExtent2D Extent;

    bool operator==(const KtViewport& other) const
    {
        return Offset.x == other.Offset.x
            && Offset.y == other.Offset.y
            && Extent.width == other.Extent.width
            && Extent.height == other.Extent.height;
    }
};

namespace std
{
    template<> struct hash<KtViewport>
    {
        size_t operator()(KtViewport const& viewport) const
        {
            const size_t h1 = hash<int32_t>()(viewport.Offset.x);
            const size_t h2 = hash<int32_t>()(viewport.Offset.y);
            const size_t h3 = hash<uint32_t>()(viewport.Extent.width);
            const size_t h4 = hash<uint32_t>()(viewport.Extent.height);

            // Combine the hash values using XOR and shifts
            return ((h1 ^ (h2 << 1)) >> 1) ^ (h3 << 1) ^ (h4 << 1);
        }
    };
}

static void ktCmdUseViewport(VkCommandBuffer commandBuffer, const KtViewport& viewport)
{
	// Sets the render region
	VkViewport vkViewport{};
	vkViewport.x = static_cast<float>(viewport.Offset.x);
	vkViewport.y = static_cast<float>(viewport.Offset.y);
	vkViewport.width = static_cast<float>(viewport.Extent.width);
	vkViewport.height = static_cast<float>(viewport.Extent.height);
	vkViewport.minDepth = 0.0f;
	vkViewport.maxDepth = 1.0f;
	vkCmdSetViewport(commandBuffer, 0, 1, &vkViewport);
	
    // Crops the render region
	VkRect2D vkScissor{};
	vkScissor.offset = viewport.Offset;
	vkScissor.extent = viewport.Extent;
	vkCmdSetScissor(commandBuffer, 0, 1, &vkScissor);
}