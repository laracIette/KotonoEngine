#include "Row.h"
#include <algorithm>
#include <kotono_common/bitwise_utils.h>
#include <glm/common.hpp>
#include <ranges>

UWidgetDisplaySettings WRow::GetContentDisplaySettings(UWidgetDisplaySettings displaySettings) const
{
	glm::vec2 size{ 0.0f, 0.0f };

	for (auto& child : children_)
	{
		if (child)
		{
			const auto childSettings{ child->GetContentDisplaySettings(displaySettings) };
			size.x += childSettings.bounds.x;
			size.y = std::max(size.y, childSettings.bounds.y);
		}
	}

	if (GetValidChildrenCount() > 1)
	{
		size.x += spacing_ * static_cast<float>(GetValidChildrenCount() - 1);
	}

	displaySettings.bounds = glm::min(displaySettings.bounds, size);
	return displaySettings;
}

glm::vec2 WRow::GetDesiredSize(glm::vec2 bounds) const
{
	glm::vec2 size{ 0.0f, 0.0f };

	for (auto& child : children_)
	{
		if (child)
		{
			const auto childDesiredSize{ child->GetDesiredSize(bounds) };
			size.x += childDesiredSize.x;
			size.y = std::max(size.y, childDesiredSize.y);
		}
	}

	if (GetValidChildrenCount() > 1)
	{
		size.x += spacing_ * static_cast<float>(GetValidChildrenCount() - 1);
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

float WRow::GetSpacing() const
{
	return spacing_;
}

void WRow::SetSpacing(const float spacing)
{
	SetState([this, spacing]() { spacing_ = spacing; });
}

void WRow::DisplayInternal(UWidgetDisplaySettings displaySettings)
{
	// Get non-expand width
	float nonExpandWidth{ 0.0f };
	for (auto& child : children_)
	{
		// Check if not horizontal expand
		if (child && !has_flag(child->GetExpand(), EExpand::Horizontal))
		{
			nonExpandWidth += child->GetContentDisplaySettings(displaySettings).bounds.x;
		}
	}

	// Get expand width
	float expandWidth{ displaySettings.bounds.x - nonExpandWidth };
	if (!children_.empty())
	{
		expandWidth -= spacing_ * static_cast<float>(children_.size() - 1);
	}
	if (const size expandCount{ GetExpandCount() })
	{
		expandWidth /= static_cast<float>(expandCount);
	}

	++displaySettings.layer;

	for (auto& child : children_)
	{
		if (child)
		{
			auto settings{ displaySettings };

			if (has_flag(child->GetExpand(), EExpand::Horizontal))
			{
				settings.bounds.x = expandWidth;
			}

			child->Display(settings);
			const auto childSettings{ child->GetContentDisplaySettings(settings) };

			displaySettings.position.x += childSettings.bounds.x;
			displaySettings.position.x += spacing_;

			displaySettings.bounds.x -= childSettings.bounds.x;
			displaySettings.bounds.x -= spacing_;
		}
	}
}
 
size WRow::GetExpandCount() const
{
	return std::ranges::count_if(children_.begin(), children_.end(),
		[](const WidgetPtr& child) { return child && has_flag(child->GetExpand(), EExpand::Horizontal); }
	);
}

#include "generated/Row.generated.inl"
