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

void WRow::Destroy()
{
	for (auto* child : rowSettings_.children)
	{
		if (child)
		{
			child->Destroy();
		}
	}

	WWidget::Destroy();
}
