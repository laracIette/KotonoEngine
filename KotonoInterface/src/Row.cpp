#include "Row.h"
#include "Expanded.h"

WRow::WRow(const RowSettings& rowSettings) :
	WChildrenOwnerWidget(rowSettings.children),
	rowSettings_(rowSettings)
{
}

void WRow::Display(DisplaySettings displaySettings)
{
	SetDisplaySettings(displaySettings);
	displaySettings = GetDisplaySettings(displaySettings);

	size_t expandedCount{ 0 };
	float nonExpandedwidth{ 0.0f };
	for (const auto* child : rowSettings_.children)
	{
		if (child)
		{
			if (dynamic_cast<const WExpanded*>(child))
			{
				++expandedCount;
			}
			else
			{
				nonExpandedwidth += child->GetDisplaySettings(displaySettings).bounds.x;
			}
		}
	}

	float expandedwidth{ displaySettings.bounds.x - nonExpandedwidth };
	if (!rowSettings_.children.empty())
	{
		expandedwidth -= rowSettings_.spacing * static_cast<float>(rowSettings_.children.size() - 1);
	}
	if (expandedCount > 0)
	{
		expandedwidth /= static_cast<float>(expandedCount);
	}

	++displaySettings.layer;

	for (auto* child : rowSettings_.children)
	{
		if (child)
		{
			auto settings{ displaySettings };

			if (dynamic_cast<WExpanded*>(child))
			{
				settings.bounds.x = expandedwidth;
				child->Display(settings);
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
 