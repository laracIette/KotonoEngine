#include "Row.h"
#include <algorithm>
#include <kotono_common/bitwise_utils.h>
#include <glm/common.hpp>
#include <ranges>

UWidgetDisplaySettings WRow::GetDisplaySettings(UWidgetDisplaySettings displaySettings) const
{
	glm::vec2 size{ 0.0f, 0.0f };

	for (auto& child : children_)
	{
		if (child)
		{
			const auto childSettings{ child->GetDisplaySettings(displaySettings) };
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
	glm::vec2 size{};

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

float WRow::GetSpacing() const
{
	return spacing_;
}

void WRow::SetSpacing(const float spacing)
{
	spacing_ = spacing;
}

void WRow::DisplayInternal(UWidgetDisplaySettings displaySettings)
{
	// Get non-flex width
	float nonFlexWidth{ 0.0f };
	for (auto& child : children_)
	{
		// Check if not horizontal flex
		if (child && !has_flag(child->GetFlex(), EFlex::Horizontal))
		{
			nonFlexWidth += child->GetDisplaySettings(displaySettings).bounds.x;
		}
	}

	// Get flex width
	float flexWidth{ displaySettings.bounds.x - nonFlexWidth };
	if (!children_.empty())
	{
		flexWidth -= spacing_ * static_cast<float>(children_.size() - 1);
	}
	if (const size flexCount{ GetFlexCount() })
	{
		flexWidth /= static_cast<float>(flexCount);
	}

	++displaySettings.layer;

	for (auto& child : children_)
	{
		if (child)
		{
			auto settings{ displaySettings };

			if (has_flag(child->GetFlex(), EFlex::Horizontal))
			{
				settings.bounds.x = flexWidth;
			}

			child->Display(settings);
			const auto childSettings{ child->GetDisplaySettings(settings) };

			displaySettings.position.x += childSettings.bounds.x;
			displaySettings.position.x += spacing_;

			displaySettings.bounds.x -= childSettings.bounds.x;
			displaySettings.bounds.x -= spacing_;
		}
	}
}
 
size WRow::GetFlexCount() const
{
	return std::ranges::count_if(children_.begin(), children_.end(),
		[](const WidgetPtr& child) { return child && has_flag(child->GetFlex(), EFlex::Horizontal); }
	);
}

#include "generated/Row.generated.inl"
