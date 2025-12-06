#include "Row.h"
#include "Expanded.h"
#include "Spacer.h"
#include <algorithm>
#include <kotono_common/bitwise_utils.h>
#include <glm/common.hpp>

WRow::WRow(const RowSettings& rowSettings) :
	WChildrenOwnerWidget(rowSettings.children),
	rowSettings_(rowSettings)
{
}

void WRow::DisplayInternal(DisplaySettings displaySettings)
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
	if (const size_t flexCount = GetFlexCount())
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

			displaySettings.position.x += child->GetDisplaySettings(settings).bounds.x;
			displaySettings.position.x += rowSettings_.spacing;

			displaySettings.bounds.x -= child->GetDisplaySettings(settings).bounds.x;
			displaySettings.bounds.x -= rowSettings_.spacing;
		}
	}
}

WWidget::DisplaySettings WRow::GetDisplaySettings(DisplaySettings displaySettings) const
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

	if (!rowSettings_.children.empty())
	{
		size.x += rowSettings_.spacing * static_cast<float>(rowSettings_.children.size() - 1);
	}

	displaySettings.bounds = glm::min(size, displaySettings.bounds);
	return displaySettings;
}
 
size_t WRow::GetFlexCount() const
{
	return std::count_if(rowSettings_.children.begin(), rowSettings_.children.end(),
		[](const WWidget* child) { return child && has_flag(child->GetFlex(), EFlex::Horizontal); }
	);
}
