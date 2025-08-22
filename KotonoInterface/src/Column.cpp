#include "Column.h"
#include "ColumnView.h"

WColumn::WColumn(const ColumnSettings& columnSettings) : 
	WChildrenOwnerWidget(columnSettings.children),
	columnSettings_(columnSettings)
{
}

VView* WColumn::CreateView()
{
	return new VColumnView(this);
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

	WWidget::Destroy();
}
