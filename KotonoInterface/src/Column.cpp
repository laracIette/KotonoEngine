#include "Column.h"

WColumn::WColumn(const ColumnSettings& columnSettings) : 
	columnSettings_(columnSettings)
{
}

void WColumn::Build(BuildSettings buildSettings)
{
	for (auto* child : columnSettings_.children)
	{
		if (child)
		{
			child->Build(buildSettings);
		}
		buildSettings.position.y += columnSettings_.spacing;
	}
}

void WColumn::Destroy()
{
	for (auto* child : columnSettings_.children)
	{
		if (child)
		{
			child->Destroy();
		}
	}
}
