#include "Column.h"

WColumn::WColumn(const ColumnSettings& columnSettings) : 
	WChildrenOwnerWidget(columnSettings.children),
	columnSettings_(columnSettings)
{
}

void WColumn::Display(DisplaySettings displaySettings)
{
	SetDisplaySettings(displaySettings);

	if (columnSettings_.children.empty())
	{
		return;
	}

	const auto count{ columnSettings_.children.size() };

	displaySettings.position.y -= displaySettings.bounds.y / 2.0f;

	if (count > 1)
	{
		displaySettings.bounds.y -= columnSettings_.spacing * static_cast<float>(count - 1);
		displaySettings.bounds.y /= static_cast<float>(count);
	}

	displaySettings.position.y += displaySettings.bounds.y / 2.0f;

	++displaySettings.layer;

	for (auto* child : columnSettings_.children)
	{
		if (child)
		{
			child->Display(displaySettings);
			displaySettings.position.y += child->GetSize().y + columnSettings_.spacing;
		}
	}
}

WWidget::DisplaySettings WColumn::GetDisplaySettings(DisplaySettings displaySettings)
{
	float height{ 0.0f };

	for (auto* child : columnSettings_.children)
	{
		if (child)
		{
			const auto childSettings{ child->GetDisplaySettings(displaySettings) };
			height += childSettings.bounds.y;
		}
	}
	displaySettings.bounds.y = std::min(height, displaySettings.bounds.y);
	return displaySettings;
}
