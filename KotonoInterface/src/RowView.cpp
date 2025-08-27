#include "RowView.h"
#include "Row.h"

VRowView::VRowView(WRow* row) :
	VView(row)
{
}

void VRowView::Build(UBuildSettings buildSettings)
{
	if (!widget_)
	{
		return;
	}

	VView::Build(buildSettings);

	auto* asRow{ static_cast<WRow*>(widget_) };

	if (asRow->rowSettings_.children.empty())
	{
		return;
	}

	const auto count{ asRow->rowSettings_.children.size() };

	buildSettings.position.x -= buildSettings.bounds.x / 2.0f;

	if (count > 1)
	{
		buildSettings.bounds.x -= asRow->rowSettings_.spacing * static_cast<float>(count - 1);
		buildSettings.bounds.x /= static_cast<float>(count);
	}

	buildSettings.position.x += buildSettings.bounds.x / 2.0f;

	++buildSettings.layer;

	for (auto* child : asRow->rowSettings_.children)
	{
		if (child)
		{
			child->CreateView()->Build(buildSettings);
			buildSettings.position.x += buildSettings.bounds.x + asRow->rowSettings_.spacing;
			//buildSettings.position.x += child->GetSize().x + rowSettings_.spacing;
		}
	}
}
