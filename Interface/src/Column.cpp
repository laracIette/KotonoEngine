#include "Column.h"
#include <algorithm>
#include <kotono_common/bitwise_utils.h>
#include <glm/common.hpp>

UWidgetDisplaySettings WColumn::GetDisplaySettings(UWidgetDisplaySettings displaySettings) const
{
	glm::vec2 size{ 0.0f, 0.0f };

	for (auto& child : children_)
	{
		if (child)
		{
			const auto childSettings{ child->GetDisplaySettings(displaySettings) };
			size.x = std::max(size.x, childSettings.bounds.x);
			size.y += childSettings.bounds.y;
		}
	}

	if (GetValidChildrenCount() > 1)
	{
		size.y += spacing_ * static_cast<float>(GetValidChildrenCount() - 1);
	}

	displaySettings.bounds = glm::min(displaySettings.bounds, size);
	return displaySettings;
}

glm::vec2 WColumn::GetDesiredSize(glm::vec2 bounds) const
{
	glm::vec2 size{};

	for (auto& child : children_)
	{
		if (child)
		{
			const auto childDesiredSize{ child->GetDesiredSize(bounds) };
			size.x = std::max(size.x, childDesiredSize.x);
			size.y += childDesiredSize.y;
		}
	}

	if (GetValidChildrenCount() > 1)
	{
		size.y += spacing_ * static_cast<float>(GetValidChildrenCount() - 1);
	}

	return size;
}

float WColumn::GetSpacing() const
{
	return spacing_;
}

void WColumn::SetSpacing(const float spacing)
{
	spacing_ = spacing;
}

void WColumn::DisplayInternal(UWidgetDisplaySettings displaySettings)
{
	// Get non-flex height
	float nonFlexHeight{ 0.0f };
	for (const auto& child : children_)
	{
		// Check if not vertical flex
		if (child && !has_flag(child->GetFlex(), EFlex::Vertical))
		{
			nonFlexHeight += child->GetDisplaySettings(displaySettings).bounds.y;
		}
	}

	// Get flex height
	float flexHeight{ displaySettings.bounds.y - nonFlexHeight };
	if (!children_.empty())
	{
		flexHeight -= spacing_ * static_cast<float>(children_.size() - 1);
	}
	if (const size flexCount{ GetFlexCount() })
	{
		flexHeight /= static_cast<float>(flexCount);
	}

	++displaySettings.layer;

	for (auto& child : children_)
	{
		if (child)
		{
			auto settings{ displaySettings };

			if (has_flag(child->GetFlex(), EFlex::Vertical))
			{
				settings.bounds.y = flexHeight;
			}

			child->Display(settings);
			const auto childSettings{ child->GetDisplaySettings(settings) };

			displaySettings.position.y += childSettings.bounds.y;
			displaySettings.position.y += spacing_;

			displaySettings.bounds.y -= childSettings.bounds.y;
			displaySettings.bounds.y -= spacing_;
		}
	}
}

size WColumn::GetFlexCount() const
{
	return std::count_if(children_.begin(), children_.end(),
		[](const WidgetPtr& child) { return child && has_flag(child->GetFlex(), EFlex::Vertical); }
	);
}

#include "generated/Column.generated.inl"
