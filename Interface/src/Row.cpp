#include "Row.h"

#include <algorithm>
#include <glm/common.hpp>
#include <kotono_common/enum_utils.h>
#include <ranges>

WRow::WRow(f32 spacing)
	: spacing_{ spacing }
{
}

glm::vec2 WRow::GetContentSize(glm::vec2 bounds) const
{
	glm::vec2 size{ 0.0f, 0.0f };

	for (auto const& child : GetChildren())
	{
		if (child)
		{
			auto const childSize{ child->GetContentSize(bounds) };
			size.x += childSize.x;
			size.y = std::max(size.y, childSize.y);
		}
	}

	if (GetValidChildrenCount() > 1)
	{
		size.x += spacing_ * static_cast<f32>(GetValidChildrenCount() - 1);
	}

	bounds = glm::min(bounds, size);
	return bounds;
}

glm::vec2 WRow::GetDesiredSize(glm::vec2 const& bounds) const
{
	glm::vec2 size{ 0.0f, 0.0f };

	for (auto const& child : GetChildren())
	{
		if (child)
		{
			auto const childDesiredSize{ child->GetDesiredSize(bounds) };
			size.x += childDesiredSize.x;
			size.y = std::max(size.y, childDesiredSize.y);
		}
	}

	if (GetValidChildrenCount() > 1)
	{
		size.x += spacing_ * static_cast<f32>(GetValidChildrenCount() - 1);
	}

	return size;
}

EExpand WRow::GetExpand() const
{
	return EExpand::Horizontal;
}

EFlex WRow::GetFlex() const
{
	return EFlex::All;
}

void WRow::DisplayInternal(UWidgetDisplaySettings displaySettings)
{
	// Get non-expand width
	f32 nonExpandWidth{ 0.0f };
	for (auto const& child : GetChildren())
	{
		// Check if not horizontal expand
		if (child && !has_flag(child->GetExpand(), EExpand::Horizontal))
		{
			auto const childSize{ child->GetContentSize(displaySettings.bounds) };
			nonExpandWidth += childSize.x;
		}
	}

	// Get expand width
	f32 expandWidth{ displaySettings.bounds.x - nonExpandWidth };
	if (!GetChildren().empty())
	{
		expandWidth -= spacing_ * static_cast<f32>(GetChildren().size() - 1);
	}
	if (size const expandCount{ GetExpandCount() })
	{
		expandWidth /= static_cast<f32>(expandCount);
	}

	++displaySettings.layer;

	for (auto const& child : GetChildren())
	{
		if (child)
		{
			auto settings{ displaySettings };

			if (has_flag(child->GetExpand(), EExpand::Horizontal))
			{
				settings.bounds.x = expandWidth;
			}

			child->Display(settings);
			auto const childSize{ child->GetContentSize(settings.bounds) };

			displaySettings.position.x += childSize.x;
			displaySettings.position.x += spacing_;

			displaySettings.bounds.x -= childSize.x;
			displaySettings.bounds.x -= spacing_;
		}
	}
}
 
size WRow::GetExpandCount() const
{
	return std::ranges::count_if(GetChildren(),
		[](WidgetPtr const& child) { return child && has_flag(child->GetExpand(), EExpand::Horizontal); }
	);
}

#include "generated/Row.generated.inl"
