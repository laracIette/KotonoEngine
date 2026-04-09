#include "Row.h"
#include "Expanded.h"
#include "Spacer.h"
#include <algorithm>
#include <kotono_common/bitwise_utils.h>
#include <glm/common.hpp>
#include <ranges>

WRow::WRow(const RowSettings& rowSettings) :
	WChildrenOwner(rowSettings.children),
	rowSettings_(rowSettings)
{
}

void WRow::DisplayInternal(UWidgetDisplaySettings displaySettings)
{
	// Get non-flex width
	float nonFlexWidth{ 0.0f };
	for (const auto* child : rowSettings_.children)
	{
		// Check if not horizontal flex
		if (child && !has_flag(child->GetFlex(), EFlex::Horizontal))
		{
			nonFlexWidth += child->GetDisplaySettings(displaySettings).bounds.x;
		}
	}

	// Get flex width
	float flexWidth{ displaySettings.bounds.x - nonFlexWidth };
	if (!rowSettings_.children.empty())
	{
		flexWidth -= rowSettings_.spacing * static_cast<float>(rowSettings_.children.size() - 1);
	}
	if (const size flexCount{ GetFlexCount() })
	{
		flexWidth /= static_cast<float>(flexCount);
	}

	++displaySettings.layer;

	for (auto* child : rowSettings_.children)
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
			displaySettings.position.x += rowSettings_.spacing;

			displaySettings.bounds.x -= childSettings.bounds.x;
			displaySettings.bounds.x -= rowSettings_.spacing;
		}
	}
}

UWidgetDisplaySettings WRow::GetDisplaySettings(UWidgetDisplaySettings displaySettings) const
{
	glm::vec2 size{ 0.0f, 0.0f };

	for (auto* child : rowSettings_.children)
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
		size.x += rowSettings_.spacing * static_cast<float>(GetValidChildrenCount() - 1);
	}

	displaySettings.bounds = glm::min(displaySettings.bounds, size);
	return displaySettings;
}

glm::vec2 WRow::GetDesiredSize(glm::vec2 bounds) const
{
	glm::vec2 size{};

	for (auto* child : rowSettings_.children)
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
		size.x += rowSettings_.spacing * static_cast<float>(GetValidChildrenCount() - 1);
	}

	return size;
}
 
size WRow::GetFlexCount() const
{
	return std::ranges::count_if(rowSettings_.children.begin(), rowSettings_.children.end(),
		[](const WidgetPtr child) { return child && has_flag(child->GetFlex(), EFlex::Horizontal); }
	);
}

#include "generated/Row.generated.inl"
