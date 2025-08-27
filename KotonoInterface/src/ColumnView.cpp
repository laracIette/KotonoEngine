#include "ColumnView.h"
#include "Column.h"

VColumnView::VColumnView(WColumn* widget) :
	VView(widget)
{
}

void VColumnView::Build(UBuildSettings buildSettings)
{
	if (!widget_)
	{
		return;
	}

	VView::Build(buildSettings);

	auto* asColumn{ static_cast<WColumn*>(widget_) };

	if (asColumn->columnSettings_.children.empty())
	{
		return;
	}

	const auto count{ asColumn->columnSettings_.children.size() };

	buildSettings.position.y -= buildSettings.bounds.y / 2.0f;

	if (count > 1)
	{
		buildSettings.bounds.y -= asColumn->columnSettings_.spacing * static_cast<float>(count - 1);
		buildSettings.bounds.y /= static_cast<float>(count);
	}

	buildSettings.position.y += buildSettings.bounds.y / 2.0f;

	++buildSettings.layer;

	for (auto* child : asColumn->columnSettings_.children)
	{
		if (child)
		{
			child->CreateView()->Build(buildSettings);
			buildSettings.position.y += buildSettings.bounds.y + asColumn->columnSettings_.spacing;
			//buildSettings.position.y += child->GetSize().y + columnSettings_.spacing;
		}
	}
}
