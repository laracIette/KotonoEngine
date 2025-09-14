#include "Column.h"
#include "Expanded.h"
#include <algorithm>

WColumn::WColumn(const ColumnSettings& columnSettings) : 
	WChildrenOwnerWidget(columnSettings.children),
	columnSettings_(columnSettings)
{
}

void WColumn::Display(DisplaySettings displaySettings)
{
	SetDisplaySettings(displaySettings);
	displaySettings = GetDisplaySettings(displaySettings);

	// Get non-expanded height
	float nonExpandedHeight{ 0.0f };
	for (const auto* child : columnSettings_.children)
	{
		if (child && !dynamic_cast<const WExpanded*>(child))
		{
			nonExpandedHeight += child->GetDisplaySettings(displaySettings).bounds.y;
		}
	}

	// Get expanded height
	float expandedHeight{ displaySettings.bounds.y - nonExpandedHeight };
	if (!columnSettings_.children.empty())
	{
		expandedHeight -= columnSettings_.spacing * static_cast<float>(columnSettings_.children.size() - 1);
	}
	if (size_t expandedCount = GetExpandedCount())
	{
		expandedHeight /= static_cast<float>(expandedCount);
	}

	++displaySettings.layer;

	for (auto* child : columnSettings_.children)
	{
		if (child)
		{
			auto settings{ displaySettings };

			if (dynamic_cast<const WExpanded*>(child))
			{
				settings.bounds.y = expandedHeight;
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
	// Return the same display settings because an expanded would fill any free space
	if (std::any_of(
			columnSettings_.children.begin(), columnSettings_.children.end(),
			[](const WWidget* child) 
			{ 
				return dynamic_cast<const WExpanded*>(child); 
			}
	))
	{
		return displaySettings;
	}

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
