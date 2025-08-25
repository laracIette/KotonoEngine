#include "Row.h"
#include "RowView.h"

WRow::WRow(const RowSettings& rowSettings) :
	WChildrenOwnerWidget(rowSettings.children),
	rowSettings_(rowSettings)
{
}

VView* WRow::CreateView()
{
	return new VRowView(this);
}
 