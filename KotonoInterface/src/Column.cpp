#include "Column.h"

WColumn::WColumn(const ColumnSettings& columnSettings) : 
	columnSettings_(columnSettings)
{
}

void WColumn::Build(BuildSettings buildSettings)
{
	WWidget::Build(buildSettings);

	for (auto* child : columnSettings_.children)
	{
		if (child)
		{
			++buildSettings.layer;
			child->Build(buildSettings);
			buildSettings.position.y += child->GetSize().y + columnSettings_.spacing;
		}
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
