#include "Column.h"
#include "Expanded.h"
#include <algorithm>
#include <kotono_framework/bitwise_utils.h>

WColumn::WColumn(const ColumnSettings& columnSettings) : 
	WChildrenOwnerWidget(columnSettings.children),
	columnSettings_(columnSettings)
{
}

void WColumn::DisplayInternal(DisplaySettings displaySettings)
{
	// Get non-flex height
	float nonFlexHeight{ 0.0f };
	for (const auto* child : columnSettings_.children)
	{
		// Check if not vertical flex
		if (child && !has_flag(child->GetFlex(), EFlex::Vertical))
		{
			nonFlexHeight += child->GetDisplaySettings(displaySettings).bounds.y;
		}
	}

	// Get flex height
	float flexHeight{ displaySettings.bounds.y - nonFlexHeight };
	if (!columnSettings_.children.empty())
	{
		flexHeight -= columnSettings_.spacing * static_cast<float>(columnSettings_.children.size() - 1);
	}
	if (const size_t flexCount = GetFlexCount())
	{
		flexHeight /= static_cast<float>(flexCount);
	}

	++displaySettings.layer;

	for (auto* child : columnSettings_.children)
	{
		if (child)
		{
			auto settings{ displaySettings };

			if (has_flag(child->GetFlex(), EFlex::Vertical))
			{
				settings.bounds.y = flexHeight;
			}

			child->Display(settings);

			displaySettings.position.y += child->GetDisplaySettings(settings).bounds.y;
			displaySettings.position.y += columnSettings_.spacing;

			displaySettings.bounds.y -= child->GetDisplaySettings(settings).bounds.y;
			displaySettings.bounds.y -= columnSettings_.spacing;
		}
	}
}

WWidget::DisplaySettings WColumn::GetDisplaySettings(DisplaySettings displaySettings) const
{
	glm::vec2 size{ 0.0f, 0.0f };

	for (auto* child : columnSettings_.children)
	{
		if (child)
		{
			const auto childSettings{ child->GetDisplaySettings(displaySettings) };
			size.x = std::max(size.x, childSettings.bounds.x);
			size.y += childSettings.bounds.y;
		}
	}

	if (!columnSettings_.children.empty())
	{
		size.y += columnSettings_.spacing * static_cast<float>(columnSettings_.children.size() - 1);
	}

	displaySettings.bounds = glm::min(size, displaySettings.bounds);
	return displaySettings;
}

size_t WColumn::GetFlexCount() const
{
	return std::count_if(columnSettings_.children.begin(), columnSettings_.children.end(),
		[](const WWidget* child) { return child && has_flag(child->GetFlex(), EFlex::Vertical); }
	);
}
