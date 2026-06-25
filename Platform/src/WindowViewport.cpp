#include "WindowViewport.h"

const glm::uvec2& KtWindowViewport::GetExtent() const
{
	return extent_;
}

const glm::ivec2& KtWindowViewport::GetOffset() const
{
    return offset_;
}

bool KtWindowViewport::GetIsKeepAspectRatio() const
{
	return isKeepAspectRatio_;
}

float KtWindowViewport::GetAspectRatio() const
{
	return aspectRatio_;
}

void KtWindowViewport::SetExtent(const glm::uvec2& extent)
{
	if (isKeepAspectRatio_)
	{
		u32 width{ extent.x };
		u32 height{ extent.y };

		if (width > height * aspectRatio_)
		{
			width = static_cast<u32>(height * aspectRatio_);
		}
		else if (height > width / aspectRatio_)
		{
			height = static_cast<u32>(width / aspectRatio_);
		}

		extent_ = { width, height };
		return;
	}

	if (extent == extent_)
	{
		return;
	}

    extent_ = extent;
	aspectRatio_ = static_cast<float>(extent.x) / extent.y;
	eventExtentChanged_.Broadcast();
}

void KtWindowViewport::SetOffset(const glm::ivec2& offset)
{
    offset_ = offset;
}

void KtWindowViewport::SetIsKeepAspectRatio(const bool isKeepAspectRatio)
{
	isKeepAspectRatio_ = isKeepAspectRatio;
}

void KtWindowViewport::SetAspectRatio(const float aspectRatio)
{
	aspectRatio_ = aspectRatio;
}

void KtWindowViewport::CmdUse(VkCommandBuffer commandBuffer) const
{
	// Sets the render region
	const VkViewport vkViewport{
		.x = static_cast<float>(offset_.x),
		.y = static_cast<float>(offset_.y),
		.width = static_cast<float>(extent_.x),
		.height = static_cast<float>(extent_.y),
		.minDepth = 0.0f,
		.maxDepth = 1.0f,
	};
	vkCmdSetViewport(commandBuffer, 0, 1, &vkViewport);

	// Crops the render region
	const VkRect2D vkScissor{
		.offset {
			.x = offset_.x,
			.y = offset_.y,
		},
		.extent{
			.width = extent_.x,
			.height = extent_.y,
		},
	};
	vkCmdSetScissor(commandBuffer, 0, 1, &vkScissor);
}

UEvent<>& KtWindowViewport::EventExtentChanged()
{
	return eventExtentChanged_;
}
