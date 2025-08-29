#include "Row.h"

WRow::WRow(const RowSettings& rowSettings) :
	WChildrenOwnerWidget(rowSettings.children),
	rowSettings_(rowSettings)
{
}

void WRow::Display(DisplaySettings displaySettings)
{
	SetDisplaySettings(displaySettings);

	if (rowSettings_.children.empty())
	{
		return;
	}

	const auto count{ rowSettings_.children.size() };

	displaySettings.position.x -= displaySettings.bounds.x / 2.0f;

	if (count > 1)
	{
		displaySettings.bounds.x -= rowSettings_.spacing * static_cast<float>(count - 1);
		displaySettings.bounds.x /= static_cast<float>(count);
	}

	displaySettings.position.x += displaySettings.bounds.x / 2.0f;

	++displaySettings.layer;

	for (auto* child : rowSettings_.children)
	{
		if (child)
		{
			child->Display(displaySettings);
			displaySettings.position.x += child->GetSize().x + rowSettings_.spacing;
		}
	}
}

WWidget::DisplaySettings WRow::GetDisplaySettings(DisplaySettings displaySettings)
{
	float width{ 0.0f };

	for (auto* child : rowSettings_.children)
	{
		if (child)
		{
			const auto childSettings{ child->GetDisplaySettings(displaySettings) };
			width += childSettings.bounds.x;
		}
	}
	displaySettings.bounds.x = std::min(width, displaySettings.bounds.x);
	return displaySettings;
}
 