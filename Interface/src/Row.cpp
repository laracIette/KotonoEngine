#include "Row.h"

#include <algorithm>
#include <glm/common.hpp>
#include <kotono_common/enum_utils.h>

auto WRow::GetContentSize(glm::vec2 const& bounds) const -> glm::vec2
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

	size = glm::min(size, bounds);
	return size;
}

auto WRow::GetDesiredSize(glm::vec2 const& bounds) const -> glm::vec2
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

auto WRow::GetExpand() const -> EExpand
{
	return std::ranges::any_of(GetChildren(),
		[](WidgetPtr const& child) { return child && has_flag(child->GetExpand(), EExpand::Vertical); }
	) ? EExpand::All : EExpand::Horizontal;
}

auto WRow::GetFlex() const -> EFlex
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
 
auto WRow::GetExpandCount() const -> size
{
	return std::ranges::count_if(GetChildren(),
		[](WidgetPtr const& child) { return child && has_flag(child->GetExpand(), EExpand::Horizontal); }
	);
}

#include "generated/Row.generated.inl"
