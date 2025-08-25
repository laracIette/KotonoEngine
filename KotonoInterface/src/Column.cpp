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
