#include "Viewport.h"

KtViewport WindowViewport;

const VkOffset2D& KtViewport::GetOffset() const
{
    return _offset;
}

const VkExtent2D& KtViewport::GetExtent() const
{
    return _extent;
}

void KtViewport::SetExtent(const VkExtent2D& extent)
{
    _extent = extent;
}

void KtViewport::SetOffset(const VkOffset2D& offset)
{
    _offset = offset;
}

void KtViewport::CmdUse(VkCommandBuffer commandBuffer) const
{
	// Sets the render region
	VkViewport vkViewport{};
	vkViewport.x = static_cast<float>(_offset.x);
	vkViewport.y = static_cast<float>(_offset.y);
	vkViewport.width = static_cast<float>(_extent.width);
	vkViewport.height = static_cast<float>(_extent.height);
	vkViewport.minDepth = 0.0f;
	vkViewport.maxDepth = 1.0f;
	vkCmdSetViewport(commandBuffer, 0, 1, &vkViewport);

	// Crops the render region
	VkRect2D vkScissor{};
	vkScissor.offset = _offset;
	vkScissor.extent = _extent;
	vkCmdSetScissor(commandBuffer, 0, 1, &vkScissor);
}
