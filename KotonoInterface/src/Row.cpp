#include "Row.h"
#include "Expanded.h"
#include <algorithm>

WRow::WRow(const RowSettings& rowSettings) :
	WChildrenOwnerWidget(rowSettings.children),
	rowSettings_(rowSettings)
{
}

void WRow::Display(DisplaySettings displaySettings)
{
	SetDisplaySettings(displaySettings);
	displaySettings = GetDisplaySettings(displaySettings);

	// Get non-expanded width
	float nonExpandedWidth{ 0.0f };
	for (const auto* child : rowSettings_.children)
	{
		if (child && !dynamic_cast<const WExpanded*>(child))
		{
			nonExpandedWidth += child->GetDisplaySettings(displaySettings).bounds.x;
		}
	}

	// Get expanded width
	float expandedWidth{ displaySettings.bounds.x - nonExpandedWidth };
	if (!rowSettings_.children.empty())
	{
		expandedWidth -= rowSettings_.spacing * static_cast<float>(rowSettings_.children.size() - 1);
	}
	if (size_t expandedCount = GetExpandedCount())
	{
		expandedWidth /= static_cast<float>(expandedCount);
	}

	++displaySettings.layer;

	for (auto* child : rowSettings_.children)
	{
		if (child)
		{
			auto settings{ displaySettings };

			if (dynamic_cast<const WExpanded*>(child))
			{
				settings.bounds.x = expandedWidth;
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
	//// Return the same display settings because an expanded would fill any free space
	//if (std::any_of(
	//	rowSettings_.children.begin(), rowSettings_.children.end(),
	//	[](const WWidget* child) {
	//		return dynamic_cast<const WExpanded*>(child);
	//	}
	//))
	//{
	//	return displaySettings;
	//}

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
 