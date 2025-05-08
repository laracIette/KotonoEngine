#include "Viewport.h"

KtViewport WindowViewport;

const glm::uvec2& KtViewport::GetExtent() const
{
	return _extent;
}

const glm::ivec2& KtViewport::GetOffset() const
{
    return _offset;
}

void KtViewport::SetExtent(const glm::uvec2& extent)
{
    _extent = extent;
}

void KtViewport::SetOffset(const glm::ivec2& offset)
{
    _offset = offset;
}

const float KtViewport::GetAspectRatio() const
{
	return static_cast<float>(_extent.x) / _extent.y;
}

void KtViewport::CmdUse(VkCommandBuffer commandBuffer) const
{
	// Sets the render region
	VkViewport vkViewport{};
	vkViewport.x = static_cast<float>(_offset.x);
	vkViewport.y = static_cast<float>(_offset.y);
	vkViewport.width = static_cast<float>(_extent.x);
	vkViewport.height = static_cast<float>(_extent.y);
	vkViewport.minDepth = 0.0f;
	vkViewport.maxDepth = 1.0f;
	vkCmdSetViewport(commandBuffer, 0, 1, &vkViewport);

	// Crops the render region
	VkRect2D vkScissor{};
	vkScissor.offset.x = _offset.x;
	vkScissor.offset.y = _offset.y;
	vkScissor.extent.width = _extent.x;
	vkScissor.extent.height = _extent.y;
	vkCmdSetScissor(commandBuffer, 0, 1, &vkScissor);
}
