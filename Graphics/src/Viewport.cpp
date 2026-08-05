#include "Viewport.h"

const glm::uvec2& UViewport::GetExtent() const
{
	return extent_;
}

const glm::ivec2& UViewport::GetOffset() const
{
    return offset_;
}

bool UViewport::GetKeepAspectRatio() const
{
	return keepAspectRatio_;
}

f32 UViewport::GetAspectRatio() const
{
	return aspectRatio_;
}

UEvent<glm::uvec2>& UViewport::GetEventExtentUpdated()
{
	return eventExtentUpdated_;
}

void UViewport::SetExtent(const glm::uvec2& extent)
{
	if (extent == extent_)
	{
		return;
	}

	if (keepAspectRatio_)
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
	}
	else
	{
		extent_ = extent;
		aspectRatio_ = static_cast<f32>(extent.x) / extent.y;
	}

	eventExtentUpdated_.Broadcast(extent_);
}

void UViewport::SetOffset(const glm::ivec2& offset)
{
    offset_ = offset;
}

void UViewport::SetKeepAspectRatio(const bool keepAspectRatio)
{
	keepAspectRatio_ = keepAspectRatio;
}

void UViewport::SetAspectRatio(const f32 aspectRatio)
{
	aspectRatio_ = aspectRatio;
}
