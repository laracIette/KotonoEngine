#include "WindowViewport.h"

glm::uvec2 SWindowViewport::extent_{};
glm::ivec2 SWindowViewport::offset_{};

bool SWindowViewport::isKeepAspectRatio_{};
f32 SWindowViewport::aspectRatio_{};

const glm::uvec2& SWindowViewport::GetExtent()
{
	return extent_;
}

const glm::ivec2& SWindowViewport::GetOffset()
{
    return offset_;
}

bool SWindowViewport::GetIsKeepAspectRatio()
{
	return isKeepAspectRatio_;
}

f32 SWindowViewport::GetAspectRatio()
{
	return aspectRatio_;
}

void SWindowViewport::SetExtent(const glm::uvec2& extent)
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
	aspectRatio_ = static_cast<f32>(extent.x) / extent.y;
}

void SWindowViewport::SetOffset(const glm::ivec2& offset)
{
    offset_ = offset;
}

void SWindowViewport::SetIsKeepAspectRatio(const bool isKeepAspectRatio)
{
	isKeepAspectRatio_ = isKeepAspectRatio;
}

void SWindowViewport::SetAspectRatio(const f32 aspectRatio)
{
	aspectRatio_ = aspectRatio;
}

void SWindowViewport::CmdUse(VkCommandBuffer commandBuffer)
{
	// Sets the render region
	const VkViewport vkViewport{
		.x = static_cast<f32>(offset_.x),
		.y = static_cast<f32>(offset_.y),
		.width = static_cast<f32>(extent_.x),
		.height = static_cast<f32>(extent_.y),
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
