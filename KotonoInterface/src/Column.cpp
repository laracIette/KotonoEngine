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

	size_t expandedCount{ 0 };
	float nonExpandedHeight{ 0.0f };
	for (const auto* child : columnSettings_.children)
	{
		if (child)
		{
			if (dynamic_cast<const WExpanded*>(child))
			{
				++expandedCount;
			}
			else
			{
				nonExpandedHeight += child->GetDisplaySettings(displaySettings).bounds.y;
			}
		}
	}

	float expandedHeight{ displaySettings.bounds.y - nonExpandedHeight };
	if (!columnSettings_.children.empty())
	{
		expandedHeight -= columnSettings_.spacing * static_cast<float>(columnSettings_.children.size() - 1);
	}
	if (expandedCount > 0)
	{
		expandedHeight /= static_cast<float>(expandedCount);
	}

	++displaySettings.layer;

	for (auto* child : columnSettings_.children)
	{
		if (child)
		{
			auto settings{ displaySettings };

			if (dynamic_cast<WExpanded*>(child))
			{
				settings.bounds.y = expandedHeight;
				child->Display(settings);
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
	glm::vec2 size{ 0.0f,0.0f };

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
