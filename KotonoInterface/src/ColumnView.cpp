#include "ColumnView.h"
#include "Column.h"

VColumnView::VColumnView(WColumn* widget) :
	VView(widget),
	column_(widget)
{
}

void VColumnView::Build(UBuildSettings buildSettings)
{
	if (!column_)
	{
		return;
	}

	VView::Build(buildSettings);

	if (column_->columnSettings_.children.empty())
	{
		return;
	}

	const auto count{ column_->columnSettings_.children.size() };

	buildSettings.position.y -= buildSettings.bounds.y / 2.0f;

	if (count > 1)
	{
		buildSettings.bounds.y -= column_->columnSettings_.spacing * static_cast<float>(count - 1);
		buildSettings.bounds.y /= static_cast<float>(count);
	}

	buildSettings.position.y += buildSettings.bounds.y / 2.0f;

	for (auto* child : column_->columnSettings_.children)
	{
		if (child)
		{
			++buildSettings.layer;
			child->CreateView()->Build(buildSettings);
			buildSettings.position.y += buildSettings.bounds.y + column_->columnSettings_.spacing;
			//buildSettings.position.y += child->GetSize().y + columnSettings_.spacing;
		}
	}
}
