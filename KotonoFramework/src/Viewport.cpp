#include "Viewport.h"

KtViewport WindowViewport;

const glm::uvec2& KtViewport::GetExtent() const
{
	return extent_;
}

const glm::ivec2& KtViewport::GetOffset() const
{
    return offset_;
}

const bool KtViewport::GetIsKeepAspectRatio() const
{
	return isKeepAspectRatio_;
}

const float KtViewport::GetAspectRatio() const
{
	return aspectRatio_;
}

void KtViewport::SetExtent(const glm::uvec2& extent)
{
	if (isKeepAspectRatio_)
	{
		uint32_t width = extent.x;
		uint32_t height = extent.y;

		if (width > height * aspectRatio_)
		{
			width = static_cast<uint32_t>(height * aspectRatio_);
		}
		else if (height > width / aspectRatio_)
		{
			height = static_cast<uint32_t>(width / aspectRatio_);
		}

		extent_ = glm::uvec2(width, height);
		return;
	}

    extent_ = extent;
	aspectRatio_ = static_cast<float>(extent.x) / extent.y;
}

void KtViewport::SetOffset(const glm::ivec2& offset)
{
    offset_ = offset;
}

void KtViewport::SetIsKeepAspectRatio(const bool isKeepAspectRatio)
{
	isKeepAspectRatio_ = isKeepAspectRatio;
}

void KtViewport::SetAspectRatio(const float aspectRatio)
{
	aspectRatio_ = aspectRatio;
}

void KtViewport::CmdUse(VkCommandBuffer commandBuffer) const
{
	// Sets the render region
	VkViewport vkViewport{};
	vkViewport.x = static_cast<float>(offset_.x);
	vkViewport.y = static_cast<float>(offset_.y);
	vkViewport.width = static_cast<float>(extent_.x);
	vkViewport.height = static_cast<float>(extent_.y);
	vkViewport.minDepth = 0.0f;
	vkViewport.maxDepth = 1.0f;
	vkCmdSetViewport(commandBuffer, 0, 1, &vkViewport);

	// Crops the render region
	VkRect2D vkScissor{};
	vkScissor.offset.x = offset_.x;
	vkScissor.offset.y = offset_.y;
	vkScissor.extent.width = extent_.x;
	vkScissor.extent.height = extent_.y;
	vkCmdSetScissor(commandBuffer, 0, 1, &vkScissor);
}
