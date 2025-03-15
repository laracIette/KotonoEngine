#include "Viewport.h"

const VkOffset2D& KtViewport::GetOffset() const
{
	return _offset;
}

const VkExtent2D& KtViewport::GetExtent() const
{
	return _extent;
}

void KtViewport::SetOffset(const VkOffset2D& offset)
{
	_offset = offset;
}

void KtViewport::SetExtent(const VkExtent2D& extent)
{
	_extent = extent;
}

void KtViewport::CmdUse(VkCommandBuffer commandBuffer) const
{
	// Sets the render region
	VkViewport viewport{};
	viewport.x = _offset.x;
	viewport.y = _offset.y;
	viewport.width = static_cast<float>(_extent.width);
	viewport.height = static_cast<float>(_extent.height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

	// Crops the render region
	VkRect2D scissor{};
	scissor.offset = _offset;
	scissor.extent = _extent;
	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
}
