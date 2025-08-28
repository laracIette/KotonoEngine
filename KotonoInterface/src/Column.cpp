#include "Column.h"

WColumn::WColumn(const ColumnSettings& columnSettings) : 
	WChildrenOwnerWidget(columnSettings.children),
	columnSettings_(columnSettings)
{
}

void WColumn::Display(BuildSettings buildSettings)
{
	SetBuildSettings(buildSettings);

	if (columnSettings_.children.empty())
	{
		return;
	}

	const auto count{ columnSettings_.children.size() };

	buildSettings.position.y -= buildSettings.bounds.y / 2.0f;

	if (count > 1)
	{
		buildSettings.bounds.y -= columnSettings_.spacing * static_cast<float>(count - 1);
		buildSettings.bounds.y /= static_cast<float>(count);
	}

	buildSettings.position.y += buildSettings.bounds.y / 2.0f;

	++buildSettings.layer;

	for (auto* child : columnSettings_.children)
	{
		if (child)
		{
			child->Display(buildSettings);
			buildSettings.position.y += buildSettings.bounds.y + columnSettings_.spacing;
			//buildSettings.position.y += child->GetSize().y + columnSettings_.spacing;
		}
	}
}
