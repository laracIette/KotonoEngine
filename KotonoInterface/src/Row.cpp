#include "Row.h"

WRow::WRow(const RowSettings& rowSettings) :
	WChildrenOwnerWidget(rowSettings.children),
	rowSettings_(rowSettings)
{
}

void WRow::Display(BuildSettings buildSettings)
{
	SetBuildSettings(buildSettings);

	if (rowSettings_.children.empty())
	{
		return;
	}

	const auto count{ rowSettings_.children.size() };

	buildSettings.position.x -= buildSettings.bounds.x / 2.0f;

	if (count > 1)
	{
		buildSettings.bounds.x -= rowSettings_.spacing * static_cast<float>(count - 1);
		buildSettings.bounds.x /= static_cast<float>(count);
	}

	buildSettings.position.x += buildSettings.bounds.x / 2.0f;

	++buildSettings.layer;

	for (auto* child : rowSettings_.children)
	{
		if (child)
		{
			child->Display(buildSettings);
			buildSettings.position.x += buildSettings.bounds.x + rowSettings_.spacing;
			//buildSettings.position.x += child->GetSize().x + rowSettings_.spacing;
		}
	}
}
 